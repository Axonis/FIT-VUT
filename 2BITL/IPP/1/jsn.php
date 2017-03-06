#!/usr/bin/php
<?php

$arguments = parser($argv, $argc);
$input = file_handler($arguments["input"], "r");
$json = json_validator($input);
$output = file_handler($arguments["output"], "w");
write($json, $arguments);

function file_handler($file, $type)
{
    //This function may return Boolean FALSE, but may also return a non-Boolean value which evaluates to FALSE.
    // Please read the section on Booleans for more information. Use the === operator for testing the return value of this function.
    if ($type == "r") {
        if (($handle = file_get_contents($file)) === false) {
            throw new errException("Failed to read input file.", 2);
        }
        return $handle;
    }

    if ($type == "w") {
        if (($handle = fopen($file, $type)) == false) {
            throw new errException("Failed to write to output file.", 3);
        } else {
            fclose($handle);
        }
        return $handle;
    }
    throw new errException("Function file_handler got different parameters than expected.", 500);
}

function json_validator($string)
{
    $json = json_decode($string, false);
    if (!is_object($json) && !is_array($string)) {
        throw new errException("Input file does not have correct JSON annotation.", 4);
    }
    return $json;
}

function parser($arguments, $count)
{
    // Array of arguments with their respective parameters (argument => parameter)
    $arg_array = array();
    // Last argument being parsed
    $parameters = array();
    // Arg 0 = name of script
    if (($count == 2) && ($arguments[1] === "--help")) {  // if there is only help defined
        help();
    }
    for ($i = 1; $i < $count; $i++) {
        if (preg_match("/^--(input|output|array-name|item-name|start)=(.*)/", $arguments[$i], $parameters)
            || preg_match("/^-(r|h)=(.*)/", $arguments[$i], $parameters) //
        ) {   // array[param[1]] (already in array) = param[2] (new parameter parsed)
            if (empty($arg_array[$parameters[1]]) && ($parameters[2] !== NULL)) { // argument=parameter already defined
                $arg_array[$parameters[1]] = $parameters[2];
            } else { // Argument already set - error
                throw new errException("Option cannot be set twice.", 1);
            }
        } elseif (preg_match("/^-(n|s|i|l|c|a|t)$/", $arguments[$i], $parameters)
            || preg_match("/^--(index-items|array-size|types)$/", $arguments[$i], $parameters)
        ) {   // when regex matches options without value, [1]is option [2]is null
            if (empty($arg_array[$parameters[1]])) {
                $arg_array[$parameters[1]] = true;
            } else {  // Argument already set - error
                throw new errException("Option cannot be set twice.", 1);
            }
        } else { // Set argument that is not defined
            throw new errException("Option not defined.", 1);
        }
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
            throw new errException("Option \"--start\" has dependency on \"--index-items\" or \"-t\".", 1);
        } elseif (!ctype_digit($arg_array["start"])) {
            throw new errException("Option \"--start\" requires unsigned integer value.", 1);
        } else {   // casting to integer
            $arg_array["start"] = (int)$arg_array["start"];
        }

    }

    // Same short and long options set at the same time
    if (!empty($arg_array["t"]) && !empty($arg_array["index-items"])) {
        throw new errException("\"-t\" and \"--index-items\" cannot be specified at the same time.", 1);
    }
    if (!empty($arg_array["a"]) && !empty($arg_array["array-size"])) {   // when -a is set --array-size can not be set it is the same option
        throw new errException("\"-a\" and \"--array-size\" cannot be specified at the same time.", 1);
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
            throw new errException("Invalid XML tag.", 50);
        }
    }

    return $arg_array;
}


/*
** initialization of writer and calling write_ functions
**/
function write($json, $arguments)
{
    $xml = new XMLWriter();
    $xml->openURI($arguments["output"]);
    $xml->setIndent(true);
    if (empty($arguments["n"])) {   // header
        $xml->startDocument("1.0", "UTF-8");
    }
    if (!empty($arguments["r"])) {   // placing result into "root-element" not valid name causes error 50
        $xml->startElement(check_name($arguments["r"], $arguments["h"], false));
        write_value($xml, $json, $arguments);
        $xml->endElement();
    } else {
        write_value($xml, $json, $arguments);
    }
}

/*
** recursively called for writing objects
*/
function write_object($xml, $object, $arguments)
{
    foreach ($object as $key => $value) {   // when string subst is invalid character causes error 51
        $xml->startElement(check_name($key, $arguments["h"], true));
        write_value($xml, $value, $arguments);
        $xml->endElement();
    }
}

/*
** recursively called for writing arrays
*/
function write_array($xml, $array, $arguments)
{
    $xml->startElement($arguments["array-name"]);
    if (!empty($arguments["array-size"]) || !empty($arguments["a"])) {
        $xml->writeAttribute("size", count($array));
    }
    $index = $arguments["start"];    // counter for -t or --index-items
    foreach ($array as $key => $value) {
        $xml->startElement($arguments["item-name"]);
        if (!empty($arguments["index-items"]) || !empty($arguments["t"])) {
            $xml->writeAttribute("index", $index++);
        }
        write_value($xml, $value, $arguments);
        $xml->endElement();
    }
    $xml->endElement();
}

/*
** function for writing values
*/
function write_value($xml, $value, $arguments)
{
    if (is_object($value)) {
        write_object($xml, $value, $arguments);
    } elseif (is_array($value)) {
        write_array($xml, $value, $arguments);
    } elseif ((is_int($value) || is_float($value))) {
        $number = floor($value); // number has to be floored
        if (!empty($arguments["i"])) {   // handling -i
            $xml->text($value);
        } else {
            $xml->writeAttribute("value", $number);
            if (!empty($arguments["types"])) {  // handling --types
                if (is_int($value)) {
                    $xml->writeAttribute("type", "integer");
                } else {
                    $xml->writeAttribute("type", "real");
                }
            }

        }
    } elseif ((empty($arguments["s"]) && is_string($value))) {
        $xml->writeAttribute("value", $value);
        if (!empty($arguments["types"])) { // handling --types
            $xml->writeAttribute("type", "string");
        }
    } elseif (($value === true || $value === false || $value === null)) {  // handling literals
        if (!empty($arguments["l"])) { // handling -l
            if ($value === true) {
                $xml->writeElement("true");
            } elseif ($value === false) {
                $xml->writeElement("false");
            } elseif ($value === null) {
                $xml->writeElement("null");
            }
        } else {
            if ($value === true) {
                $xml->writeAttribute("value", "true");
            } elseif ($value === false) {
                $xml->writeAttribute("value", "false");
            } elseif ($value === null) {
                $xml->writeAttribute("value", "null");
            }
            if (!empty($arguments["types"])) {
                $xml->writeAttribute("type", "literal");
            }

        }
    } elseif (!empty($arguments["c"])) {   // translation of problematic characters
        $xml->text($value);
    } else {
        $xml->writeRaw($value);


    }
}

/*
** function for checking names and replacing invalid characters
**/
function check_name($name, $replacement, $allow_replace)
{
    $start_char_rex = '/^[^\p{L}|\_]/';
    $validity_rex = '/<|>|"|\'|\/|\\|&|&/';
    if (preg_match($start_char_rex, $name) || preg_match($validity_rex, $name)) {   // if regex matches there is invalid character
        if ($allow_replace) {
            $name = preg_replace($validity_rex, $replacement, $name);
            $name = preg_replace($start_char_rex, $replacement, $name);
            if (preg_match($start_char_rex, $name) || preg_match($validity_rex, $name)) {
                throw new errException("Invalid name of XML element after substitution.", 51);
            }
        } else {
            throw new errException("Invalid XML tag.", 50);
        }
    }
    return $name;
}

function is_valid_xml_tag($name)
{
    try {
        new DOMElement($name);
    } catch (DOMException $e) {
        return false;
    }
    return true;
}

// Class for handling errors (errorException is not fitting)
class errException extends Exception
{
    public $code, $message;

    public function __construct($code, $message)
    {
        $this->code = $code;
        $this->message = $message;
    }

    public function __toString()
    {
        return "Error: $this->code (Error Number: $this->message)\n";
    }
}

// Function to output help
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