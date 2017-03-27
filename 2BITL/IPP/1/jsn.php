<?php

#JSN:xurban66
try {
    $arguments = parser($argv);
    $input = file_handler($arguments["input"], "r");
    $json = json_validator($input);
    $output = file_handler($arguments["output"], "w");
    xml($json, $arguments);

} catch (ErrorException $e) {
    echo $e;
    die($e->getCode());
}

/**
 * Function parses input from CLI and fills respective array counterparts
 * if given option is set. Set arguments are subsequently checked if their format
 * is correct, eventually default value for unset arguments is set.
 *
 * @param $arguments - string of arguments from CLI
 * @return array - array of arguments with their parameters
 * @throws ErrorException
 */
function parser($arguments)
{
    // Array of arguments with their respective parameters (argument => parameter)
    $arg_array = array();
    // Last argument being parsed
    $parameters = array();
    // Arg 0 = name of script

    // Unset name of script
    unset($arguments[0]);

    foreach ($arguments as $value) {
        if (preg_match('/^--(input|output|array-name|item-name|start)=(.*)/', $value, $parameters)
            or preg_match('/^-(r|h)=(.*)/', $value, $parameters) //
        ) {   // Array[param[1]] (already in array) = param[2] (new parameter parsed)
            if (empty($arg_array[$parameters[1]]) and ($parameters[2] !== NULL)) { // Argument=parameter already defined
                $arg_array[$parameters[1]] = $parameters[2];
            } else { // Argument already set - error
                throw new ErrorException("Option cannot be set twice. \"$value\" ", 1);
            }
        } elseif (preg_match('/^-(n|s|i|l|c|a|t)$/', $value, $parameters)
            or preg_match('/^--(index-items|array-size|types|help|padding)$/', $value, $parameters)
        ) {   // Set parameter[1] to true, parameter[2] stays as null
            if (empty($arg_array[$parameters[1]])) {
                $arg_array[$parameters[1]] = true;
            } else {  // Argument already set - error
                throw new ErrorException("Option cannot be set twice. \"$value\" ", 1);
            }
        } else { // Set argument that is not defined
            throw new ErrorException("Option not defined. \"$value\" ", 1);
        }
    }

    // Option help is defined
    if (!empty($arg_array["help"]) and count($arg_array) == 1) {
        help();
    } elseif (!empty($arg_array["help"])) {
        throw new ErrorException("Option \"--help\" has to be only argument.", 1);
    }

    // Options having default value and not being specified
    if (empty($arg_array["input"])) {
        $arg_array["input"] = "php://stdin";
    }
    if (empty($arg_array["output"])) {
        $arg_array["output"] = "php://stdout";
    }
    // TODO -h=""
    if (!isset($arg_array["h"])) { // Empty would not detect if subst was null
        $arg_array["h"] = "-";
    }
    if (!isset($arg_array["start"])) { // Empty would detect 0 as true
        $arg_array["start"] = 1;
    } else {
        // Check dependency of start and index-items
        if (empty($arg_array["t"]) and empty($arg_array["index-items"])) {
            throw new ErrorException("Option \"--start\" has dependency on \"--index-items\" or \"-t\".", 1);
        }
        if (!ctype_digit($arg_array["start"])) {
            throw new ErrorException("Option \"--start\" requires unsigned integer value.", 1);
        }
    }

    // Check dependency of padding and index-items
    if (!empty($arg_array["padding"])) {
        if (empty($arg_array["t"]) and empty($arg_array["index-items"])) {
            throw new ErrorException("Option \"padding\" has dependency on \"--index-items\"  or \"- t\".", 1);
        }
    }

    // Same short and long options set at the same time
    if (!empty($arg_array["t"]) and !empty($arg_array["index-items"])) {
        throw new ErrorException("\"-t\" and \"--index-items\" cannot be specified at the same time.", 1);
    }
    if (!empty($arg_array["a"]) and !empty($arg_array["array-size"])) {
        throw new ErrorException("\"-a\" and \"--array-size\" cannot be specified at the same time.", 1);
    }

    // Check whether arguments that are going to be XML tags are in correct format
    if (empty($arg_array["array-name"])) {
        $arg_array["array-name"] = "array";
    } else {
        $arg_array["array-name"] = validate_xml_name($arg_array["array-name"], $arg_array["h"], false);
    }
    if (empty($arg_array["item-name"])) {
        $arg_array["item-name"] = "item";
    } else {
        $arg_array["item-name"] = validate_xml_name($arg_array["item-name"], $arg_array["h"], false);
    }
    if (isset($arg_array["r"])) { // Could be 0 (empty would not be enough to detect it)
        if ($arg_array["r"] === ""){
            throw new ErrorException("Empty root element leads to incorrect XML tag.", 1);
        }
        validate_xml_name($arg_array["r"], $arg_array["h"], false);
    }

    return $arg_array;
}


/**
 * Function initializes XMLWriter, also handles root and header options.
 * Array of json elements is divided into smaller portions.
 * @param $json - decoded json
 * @param $arguments - parsed arguments in array with parameters
 */
function xml($json, $arguments)
{
    $xml = new XMLWriter();
    $xml->openURI($arguments["output"]);
    $xml->setIndent(true);

    if (empty($arguments["n"])) {   // Handle -n
        $xml->startDocument("1.0", "UTF-8");
    }
    if (!empty($arguments["r"])) {   // Handle -r
        $xml->startElement(validate_xml_name($arguments["r"], $arguments["h"], false));
        xml_element($xml, $json, $arguments);
        $xml->endElement();
    } else {
        xml_element($xml, $json, $arguments);
    }
}

/**
 * Function that is being recursively called in case json element is object.
 *
 * @param $xml - handle for XMLWriter
 * @param $object - json object to be handled
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_object($xml, $object, $arguments)
{
    foreach ($object as $key => $value) {
        $xml->startElement(validate_xml_name($key, $arguments["h"], true)); // Substitution for UTF-8
        xml_element($xml, $value, $arguments);
        $xml->endElement();
    }
}

/**
 * Function that is being recursively called in case json element is array.
 * Handles --array-size, --index-items and --padding options.
 *
 * @param $xml - handle for XMLWriter
 * @param $array - array to be handled in case array size option is defined
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_array($xml, $array, $arguments)
{
    $xml->startElement($arguments["array-name"]);
    if (!empty($arguments["array-size"]) or !empty($arguments["a"])) { // Option -a or --array-size is set
        $xml->writeAttribute("size", count($array));
    }

    $index = $arguments["start"]; // Counter for -t or --index-items

    if (!empty($arguments["padding"])) {
        $padding = strlen($index + count($array) - 1); // Length of number containing zeroes
    }

    foreach ($array as $key => $value) {
        $xml->startElement($arguments["item-name"]);
        if (!empty($arguments["index-items"]) or !empty($arguments["t"])) {
            if (!empty($arguments["padding"])) {
                $index = str_pad($index, $padding, '0', STR_PAD_LEFT);
            }
            $xml->writeAttribute("index", $index++);
        }
        xml_element($xml, $value, $arguments);
        $xml->endElement();
    }
    $xml->endElement();
}

/**
 * Function to differ whether given input is a object,array,number,string or literal.
 * For each type of input according function is called.  Also handles -c  option.
 *
 * @param $xml - handle for XMLWriter
 * @param $value - value of JSON element
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_element($xml, $value, $arguments)
{
    if (is_object($value)) {
        xml_object($xml, $value, $arguments);
    } elseif (is_array($value)) {
        xml_array($xml, $value, $arguments);
    } elseif ((is_int($value) or is_float($value))) {
        xml_number($xml, $value, $arguments);
    } elseif (is_string($value)) {
        xml_string($xml, $value, $arguments);
    } elseif ((is_bool($value) or $value === null)) {
        xml_literal($xml, $value, $arguments);
    } elseif (!empty($arguments["c"])) { // Handle -c
        $xml->text($value);
    } else {
        $xml->writeRaw($value);
    }
}

/**
 * Function that handles output when value of JSON element is number.
 * Handles -i and --types
 *
 * @param $xml - handle for XMLWriter
 * @param $number - value of JSON element
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_number($xml, $number, $arguments)
{
    $tmp = floor($number); // Number is floored to meet requirements
    if (!empty($arguments["i"])) {   // Handling -i
        $xml->text($number);
    } else {
        $xml->writeAttribute("value", $tmp);
        if (!empty($arguments["types"])) {  // Handling --types
            if (is_int($number)) {
                $xml->writeAttribute("type", "integer");
            } else {
                $xml->writeAttribute("type", "real");
            }
        }

    }
}

/**
 * Function that handles output when value of JSON element is string.
 * Handles -s and --types
 *
 * @param $xml - handle for XMLWriter
 * @param $string - value of JSON element
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_string($xml, $string, $arguments)
{
    if (empty($arguments["s"])) {
        $xml->writeAttribute("value", $string);
    } else {
        $xml->text($string);
    }
    if (!empty($arguments["types"])) {
        $xml->writeAttribute("type", "string");
    }
}

/**
 * Function that handles output when value of JSON element is literal.
 * Handles -l and --types
 *
 * @param $xml - handle for XMLWriter
 * @param $literal - value of JSON element
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_literal($xml, $literal, $arguments)
{
    if (!empty($arguments["l"])) { // Handling -l
        if ($literal === true) {
            $xml->writeElement("true");
        } elseif ($literal === false) {
            $xml->writeElement("false");
        } elseif ($literal === null) {
            $xml->writeElement("null");
        }
    } else {
        if ($literal === true) {
            $xml->writeAttribute("value", "true");
        } elseif ($literal === false) {
            $xml->writeAttribute("value", "false");
        } elseif ($literal === null) {
            $xml->writeAttribute("value", "null");
        }
        if (!empty($arguments["types"])) {
            $xml->writeAttribute("type", "literal");
        }

    }
}

/**
 * Function that checks validity of given xml tag name, in case
 * parameter $replace is set to true also substitutes invalid characters.
 *
 * @param $xml_name - Name of given xml tag
 * @param $substitution - Parameter that is going to substitute invalid characters
 * @param $replace - Bool value that enables substitution
 * @return mixed - correct xml_tag_name
 * @throws ErrorException
 */
function validate_xml_name($xml_name, $substitution, $replace)
{
    $valid_start_char = '/^[^\p{L}\_]/'; // Regex of invalid starting characters (\p{L} - unicode letter)
    $valid_char = '/[\{\}\[\]()|=!@#$%^`*~,+&<>"\'\/\\\]/'; // Regex of invalid characters globally in XML format

    if (preg_match($valid_start_char, $xml_name) or preg_match($valid_char, $xml_name)) { // Whenever match is found, XML is invalid
        if ($replace) {

            $xml_name = preg_replace($valid_start_char, $substitution, $xml_name); // Substitutes everything that is not letter or underscore
            $xml_name = preg_replace($valid_char, $substitution, $xml_name); // Substitutes each invalid char found in xml_name

            if (preg_match($valid_start_char, $xml_name) or preg_match($valid_char, $xml_name)) { // Whenever match is found, XML after substitution is invalid
                throw new ErrorException("Invalid name of XML element after substitution. \"$xml_name\" ", 51);
            }
        } else {
            throw new ErrorException("Invalid XML tag. \"$xml_name\" ", 50);
        }
    }
    return $xml_name;
}

/**
 * Function handles file operations
 *
 * @param $file - file to be handled
 * @param $type - type of operation with file
 * @return resource|string
 * @throws ErrorException
 */
function file_handler($file, $type)
{
    //This function may return Boolean FALSE, but may also return a non-Boolean value which evaluates to FALSE.
    // Please read the section on Booleans for more information. Use the === operator for testing the return value of this function.
    if ($type == "r") {
        if (($string = file_get_contents($file)) === false) {
            throw new ErrorException("Failed to read input file.", 2);
        }
        return $string;
    }

    if ($type == "w") {
        if (($handle = fopen($file, $type)) == false) {
            throw new ErrorException("Failed to write to output file.", 3);
        } else {
            fclose($handle);
        }
        return $handle;
    }
}

/**
 * Function validates whether input file is in required JSON annotation
 *
 * @param $string - raw string input from file
 * @return mixed - array of JSON elements
 * @throws ErrorException
 */
function json_validator($string)
{
    $json = json_decode($string, false); // Not using associative array option to handle array names as objects
    if (!json_last_error() == JSON_ERROR_NONE) {
        throw new ErrorException("Input file does not have correct JSON annotation.", 4);
    }
    return $json;
}

/**
 * Function that prints out help, kills script and returns 0
 */
function help()
{
    echo " 
    --help 
            Prints out help
    --input=filename 
            JSON file to be decoded (UTF-8)
    --output=filename 
            XML file which is encoded input JSON file (UTF-8)
    -h=subst 
            Each invalid character in XML tag is replaced with \"subst\"
    -n 
            Do not generate XML header
    -r=root-element 
            Name of pair root element containing XML result is set to \"root-element\"
    --array-name=array-element 
            Rename element containing array to \"array-element\"   
    --item-name=item-element 
            Rename element containing each item to \"item-element\"
    -s 
            String values are transformed to text elements instead of attributes
    -i 
            Number values are transformed to text elements instead of attributes
    -l 
            Literal values are transformed to text elements instead of attributes
    -c 
            Enables translation of problematic characters such as &, <, >, etc
    -a, --array-size 
            Each array is given attribute of its size
    -t, --index-items 
            Each item in array is given attribute of his index
    --start=n 
            Initialization of counter for index-items. Requires -t or --index-items
    --types 
            Wrapping element of each scalar value has attribute containing their type
    --padding
            Pads numbers of --index-items with 0 from left to obtain same length of each string
";
    die(0);
}


?>