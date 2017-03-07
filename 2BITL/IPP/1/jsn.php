#!/usr/bin/php
<?php

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
        if (($handle = file_get_contents($file)) === false) {
            throw new ErrorException("Failed to read input file.", 2);
        }
        return $handle;
    }

    if ($type == "w") {
        if (($handle = fopen($file, $type)) == false) {
            throw new ErrorException("Failed to write to output file.", 3);
        } else {
            fclose($handle);
        }
        return $handle;
    }
    throw new ErrorException("Function file_handler got different parameters than expected.", 500);
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
        if (preg_match("/^--(input|output|array-name|item-name|start)=(.*)/", $value, $parameters)
            || preg_match("/^-(r|h)=(.*)/", $value, $parameters) //
        ) {   // array[param[1]] (already in array) = param[2] (new parameter parsed)
            if (empty($arg_array[$parameters[1]]) && ($parameters[2] !== NULL)) { // argument=parameter already defined
                $arg_array[$parameters[1]] = $parameters[2];
            } else { // Argument already set - error
                throw new ErrorException("Option cannot be set twice.", 1);
            }
        } elseif (preg_match("/^-(n|s|i|l|c|a|t)$/", $value, $parameters)
            || preg_match("/^--(index-items|array-size|types|help)$/", $value, $parameters)
        ) {   // when regex matches options without value, [1]is option [2]is null
            if (empty($arg_array[$parameters[1]])) {
                $arg_array[$parameters[1]] = true;
            } else {  // Argument already set - error
                throw new ErrorException("Option cannot be set twice.", 1);
            }
        } else { // Set argument that is not defined
            throw new ErrorException("Option not defined.", 1);
        }
    }

    // Option help is defined
    if (!empty($arg_array["help"]) && count($arg_array) == 1) {
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
    if (empty($arg_array["h"])) {
        $arg_array["h"] = "-";
    }
    if (!isset($arg_array["start"])) { // Empty would detect 0 as true
        $arg_array["start"] = 1;
    } else {
        // Check dependency of start and index-items
        if (empty($arg_array["t"]) && empty($arg_array["index-items"])) {
            throw new ErrorException("Option \"--start\" has dependency on \"--index-items\" or \"-t\".", 1);
        } elseif (!ctype_digit($arg_array["start"])) {
            throw new ErrorException("Option \"--start\" requires unsigned integer value.", 1);
        } else {   // casting to integer
            $arg_array["start"] = (int)$arg_array["start"];
        }

    }

    // Same short and long options set at the same time
    if (!empty($arg_array["t"]) && !empty($arg_array["index-items"])) {
        throw new ErrorException("\"-t\" and \"--index-items\" cannot be specified at the same time.", 1);
    }
    if (!empty($arg_array["a"]) && !empty($arg_array["array-size"])) {   // when -a is set --array-size can not be set it is the same option
        throw new ErrorException("\"-a\" and \"--array-size\" cannot be specified at the same time.", 1);
    }


    // TODO CHECK

    if (empty($arg_array["array-name"])) {
        $arg_array["array-name"] = "array";
    } else {   // when array-name is changed script will check name, not valid causes error 50
        $arg_array["array-name"] = check_name($arg_array["array-name"], $arg_array["h"], false);
    }
    if (empty($arg_array["item-name"])) {
        $arg_array["item-name"] = "item";
    } else {   // when item-name is changed script will check name, not valid causes error 50
        $arg_array["item-name"] = check_name($arg_array["item-name"], $arg_array["h"], false);
    }
    // TODO -R OPTIONS

    if (isset($arg_array["r"])) { // Could be 0 (empty would not be enough to detect it)
        if (!is_valid_xml_tag($arg_array["r"])) {
            throw new ErrorException("Invalid XML tag.", 50);
        }
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

    if (empty($arguments["n"])) {   // header
        $xml->startDocument("1.0", "UTF-8");
    }
    if (!empty($arguments["r"])) {   // placing result into "root-element" not valid name causes error 50
        $xml->startElement(check_name($arguments["r"], $arguments["h"], false));
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
    foreach ($object as $key => $value) {   // when string subst is invalid character causes error 51
        $xml->startElement(check_name($key, $arguments["h"], true)); // substitution
        xml_element($xml, $value, $arguments);
        $xml->endElement();
    }
}

/**
 * Function that is being recursively called in case json element is array.
 * Handles array size and index items options.
 *
 * @param $xml - handle for XMLWriter
 * @param $array - array to be handled in case array size option is defined
 * @param $arguments - parsed arguments in array with parameters
 */
function xml_array($xml, $array, $arguments)
{
    $xml->startElement($arguments["array-name"]);
    if (!empty($arguments["array-size"]) || !empty($arguments["a"])) { // Option -a or --array-size is set
        $xml->writeAttribute("size", count($array));
    }
    $index = $arguments["start"];    // counter for -t or --index-items
    foreach ($array as $key => $value) {
        $xml->startElement($arguments["item-name"]);
        if (!empty($arguments["index-items"]) || !empty($arguments["t"])) {
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
    } elseif ((is_int($value) || is_float($value))) {
        xml_number($xml, $value, $arguments);
    } elseif (is_string($value)) {
        xml_string($xml, $value, $arguments);
    } elseif ((is_bool($value) || $value === null)) {  // handling literals
        xml_literal($xml, $value, $arguments);
    } elseif (!empty($arguments["c"])) {   // translation of problematic characters
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
    $tmp = floor($number); // number has to be floored
    if (!empty($arguments["i"])) {   // handling -i
        $xml->text($number);
    } else {
        $xml->writeAttribute("value", $tmp);
        if (!empty($arguments["types"])) {  // handling --types
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
    if (!empty($arguments["l"])) { // handling -l
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
 * @param $name
 * @param $substitution
 * @param $allow_replace
 * @return mixed
 * @throws ErrorException
 */
function check_name($name, $substitution, $allow_replace)
{
    $start_char_rex = '/^[^\p{L}|\_]/';
    $validity_rex = '/<|>|"|\'|\/|\\|&|&/';
    if (preg_match($start_char_rex, $name) || preg_match($validity_rex, $name)) {   // if regex matches there is invalid character
        if ($allow_replace) {
            $name = preg_replace($validity_rex, $substitution, $name);
            $name = preg_replace($start_char_rex, $substitution, $name);
            if (preg_match($start_char_rex, $name) || preg_match($validity_rex, $name)) {
                throw new ErrorException("Invalid name of XML element after substitution.", 51);
            }
        } else {
            throw new ErrorException("Invalid XML tag.", 50);
        }
    }
    return $name;
}

function is_valid_xml_tag($name)
{
    try {
        new DOMElement($name);
    } catch (DOMException $e) {
        throw new ErrorException("Invalid XML tag.", 50);
    }
    return true;
}

/**
 * Function that prints out help, kills script and returns 0
 */
function help()
{
    echo " 
    --help viz společné zadání všech úloh
    
    --input=filename zadaný vstupní JSON soubor v kódování UTF-8
    
    --output=filename textový výstupní XML soubor v kódování UTF-8 s obsahem převedeným
ze vstupního souboru.

    -h=subst ve jméně elementu odvozeném z dvojice jméno-hodnota nahraďte každý nepovolený
znak ve jméně XML značky řetězcem subst. Implicitně (i při nezadaném parametru -h) uvažujte
nahrazování znakem pomlčka (-). Vznikne-li po nahrazení invalidní jméno XML elementu,
skončete s chybou a návratovým kódem 51.

    -n negenerovat XML hlavičku na výstup skriptu (vhodné například v případě kombinování
více výsledků).

    -r=root-element jméno párového kořenového elementu obalujícího výsledek. Pokud nebude
zadán, tak se výsledek neobaluje kořenovým elementem, ač to potenciálně porušuje validitu
XML (skript neskončí s chybou). Zadání řetězce root-element vedoucího na nevalidní XML
značku ukončí skript s chybou a návratovým kódem 50 (nevalidní znaky nenahrazujte).

    --array-name=array-element tento parametr umožní přejmenovat element obalující pole
z implicitní hodnoty array na array-element. Zadání řetězce array-element vedoucího na
nevalidní XML značku ukončí skript s chybou a návratovým kódem 50 (nevalidní znaky nenahrazujte).

    --item-name=item-element analogicky, tímto parametrem lze změnit jméno elementu pro
prvky pole (implicitní hodnota je item). Zadání řetězce item-element vedoucího na nevalidní
XML značku ukončí skript s chybou a návratovým kódem 50 (nevalidní znaky nenahrazujte).

    -s hodnoty (v dvojici i v poli) typu string budou transformovány na textové elementy místo
atributů.

    -i hodnoty (v dvojici i v poli) typu number budou transformovány na textové elementy místo
atributů.

    -l hodnoty literálů (true, false, null) budou transformovány na elementy true,
false a null místo na atributy.

    -c tento přepínač oproti implicitnímu chování aktivuje překlad problematických znaků. Pro
XML problematické znaky s UTF-8 kódem menším jak 128 ve vstupních řetězcových hodnotách
(ve dvojicích i polích) konvertujte na odpovídající zápisy v XML pomocí metaznaku & (např.
&amp;, &lt;, &gt; atd.). Ostatní problematické znaky konvertovat nemusíte.

    -a, --array-size u pole bude doplněn atribut size s uvedením počtu prvků v tomto poli.
    
    -t, --index-items ke každému prvku pole bude přidán atribut index s určením indexu prvku
v tomto poli (číslování začíná od 1, pokud není parametrem --start určeno jinak).

    --start=n inicializace inkrementálního čitače pro indexaci prvků pole (nutno kombinovat
s parametrem --index-items, jinak chyba s návratovým kódem 1) na zadané kladné celé číslo
n včetně nuly (implicitně n = 1).

    --types obalující element každé skalární hodnoty bude doplněn o atribut type obsahující
hodnotu integer pro původní celočíselnou hodnotu, real pro původní desetinnou hodnotu,
string pro řetězec a literal pro hodnoty literálů.
";
    die(0);
}


?>