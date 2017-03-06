#!/usr/bin/php
<?php

$arguments = parser($argv, $argc);
$input = file_handler($arguments["input"],"r");
$json = json_validator($input);
$output = file_handler($arguments["output"],"w");
write($json, $arguments);

function file_handler($file, $type)
{
    //This function may return Boolean FALSE, but may also return a non-Boolean value which evaluates to FALSE.
    // Please read the section on Booleans for more information. Use the === operator for testing the return value of this function.
    if($type == "r") {
        if (($handle = file_get_contents($file)) === false) {
            throw new errException("Failed to read input file.", 2);
        }
        return $handle;
    }

    if($type == "w") {
        if (($handle = fopen($file, $type)) == false) {
            throw new errException("Failed to write to output file.",3);
        }
        else {
            fclose($handle);
        }
        return $handle;
    }
    throw new errException("Function file_handler got different parameters than expected.",500);
}

function json_validator($string)
{
    $json = json_decode($string, false);
    if(!is_object($json) && !is_array($string)){
        throw new errException("Input file does not have correct JSON annotation.",4);
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
        if (preg_match("/^--(input|output|array-name|item-name|start|types)=(.*)/", $arguments[$i], $parameters)
            || preg_match("/^-(r|h)=(.*)/", $arguments[$i], $parameters) //
        ) {   // array[param[1]] (already in array) = param[2] (new parameter parsed)
            if (empty($arg_array[$parameters[1]]) && ($parameters[2] !== NULL)) { // argument=parameter already defined
                $arg_array[$parameters[1]] = $parameters[2];
            } else { // Argument already set - error
                throw new errException("Option cannot be set twice.", 1);
            }
        } elseif (preg_match("/^-(n|s|i|l|c|a|t)$/", $arguments[$i], $parameters)
            || preg_match("/^--(index-items|array-size)$/", $arguments[$i], $parameters)
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


    return $arg_array;
}


/*
** initialization of writer and calling write_ functions
**/
function write($json_input, $args)
{
    $writer = new XMLWriter();
    $writer->openURI($args["output"]);
    $writer->setIndent(true);
    if (empty($args["n"])) {   // header
        $writer->startDocument("1.0", "UTF-8");
    }
    if (!empty($args["r"])) {   // placing result into "root-element" not valid name causes error 50
        $writer->startElement(check_name($args["r"], $args["h"], false));
        write_value($writer, $json_input, $args);
        $writer->endElement();
    } else {
        write_value($writer, $json_input, $args);
    }
}

/*
** recursively called for writing objects
*/
function write_object($writer, $object, $args)
{
    foreach ($object as $key => $value) {   // when string subst is invalid character causes error 51
        $writer->startElement(check_name($key, $args["h"], true));
        write_value($writer, $value, $args);
        $writer->endElement();
    }
}

/*
** recursively called for writing arrays
*/
function write_array($writer, $array, $args)
{
    $writer->startElement($args["array-name"]);
    if (!empty($args["array-size"]) || !empty($args["a"])) {
        $writer->writeAttribute("size", count($array));
    }
    $index = $args["start"];    // index counter initialization for every array
    foreach ($array as $key => $value) {
        $writer->startElement($args["item-name"]);
        if (!empty($args["index-items"]) || !empty($args["t"])) {
            $writer->writeAttribute("index", $index++);
        }
        write_value($writer, $value, $args);
        $writer->endElement();
    }
    $writer->endElement();
}

/*
** function for writing values
*/
function write_value($writer, $value, $args)
{
    if (is_object($value)) {
        write_object($writer, $value, $args);
    } elseif (is_array($value)) {
        write_array($writer, $value, $args);
    } elseif ((is_int($value) || is_float($value))) {
        $value = floor($value); // need to floor number values before writing
        if (!empty($args["i"])) {   // when option -i is set
            $writer->text($value);
        } else {
            $writer->writeAttribute("value", $value);
        }
    } elseif ((empty($args["s"]) && is_string($value))) {
        $writer->writeAttribute("value", $value);
    } elseif (($value === true || $value === false || $value === null)) {   // handling litterals true false and null
        if (!empty($args["l"])) {   // when -l option is set for replacing literal with <literal/> tag
            if ($value === true) {
                $writer->writeElement("true");
            } elseif ($value === false) {
                $writer->writeElement("false");
            } elseif ($value === null) {
                $writer->writeElement("null");
            }
        } else {
            if ($value === true) {
                $writer->writeAttribute("value", "true");
            } elseif ($value === false) {
                $writer->writeAttribute("value", "false");
            } elseif ($value === null) {
                $writer->writeAttribute("value", "null");
            }
        }
    } elseif (!empty($args["c"])) {   // translation of problematic characters
        $writer->text($value);
    } else {
        $writer->writeRaw($value);
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
                throw new errException("Invalid name of XML element after substitution.",51);
            }
        } else {
            throw new errException("Invalid XML tag.",50);
        }
    }
    return $name;
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