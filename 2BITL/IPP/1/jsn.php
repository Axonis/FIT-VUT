#!/usr/bin/php
<?php

//var_dump($argv);

//$test = parser::$input; // pristupovanie k premennych vnutri
//echo $test;


parser::load($argv);

if(parser::$help)
{
    help();
    die(0);
}

$handle_input = fopen(parser::$input, "r");
$handle_output = fopen(parser::$output, "w");
$input_str = fread($handle_input, filesize(parser::$input));
$input_str_json = json_decode($input_str,true);

xml::xml_encoder($input_str_json);





class parser {

    static
        // NULL for input
        // BOOL for options
        $help = FALSE,
        $input = "php://stdin",
        $output = "php://stdout",
        $substitution = "-",
        $header = TRUE,
        $root = NULL,
        $array = NULL,
        $item = NULL,
        $string = FALSE,
        $number = FALSE,
        $literal = FALSE,
        $conversion = FALSE,
        $size = FALSE,
        $items = FALSE,
        $start = 1,
        $types = FALSE,
        $dependency = FALSE;
        // TODO

    public static function load(array $argv){
        // Unsets first argument which is name of the script
        unset($argv[0]);

        foreach($argv as $arg){

            if($arg === "--help"){
                if(self::$help){
                    throw new errException(1);
                }
                self::$help = TRUE;
            }

            elseif(substr($arg, 0, 8) === "--input="){
                if(self::$input !== "php://stdin"){
                    throw new errException(1);
                }
                self::$input = substr($arg, 8);
            }

            elseif(substr($arg, 0, 9) === "--output="){
                if(self::$output !== "php://stdout"){
                    throw new errException(1);
                }
                self::$output = substr($arg, 9);
            }

            elseif($arg === "-n"){
                if(!(self::$header)){
                    throw new errException(1);
                }
                self::$header = FALSE;
            }

            elseif(substr($arg, 0, 3) === "-h="){
                if(self::$substitution !== "-"){
                    throw new errException(1);
                }
                //TODO CHECK SUBST STRING FOR ERRORS
                self::$substitution = substr($arg, 3);
            }

            elseif(substr($arg, 0, 13) === "--array-name="){
                if(self::$array !== NULL){
                    throw new errException(1);
                }
                self::$array = substr($arg, 13);
            }

            elseif(substr($arg, 0, 12) === "--item-name="){
                if(self::$item !== NULL){
                    throw new errException(1);
                }
                self::$item = substr($arg, 12);
            }

            elseif($arg === "-s"){
                if(self::$string){
                    throw new errException(1);
                }
                self::$string = TRUE;
            }

            elseif($arg === "-i"){
                if(self::$number){
                    throw new errException(1);
                }
                self::$number = TRUE;
            }

            elseif($arg === "-l"){
                if(self::$literal){
                    throw new errException(1);
                }
                self::$literal = TRUE;
            }

            elseif($arg === "-c"){
                if(self::$conversion){
                    throw new errException(1);
                }
                self::$conversion = TRUE;
            }

            elseif($arg === "-a" || $arg === "--array-size"){
                if(self::$size){
                    throw new errException(1);
                }
                self::$size = TRUE;
            }

            elseif($arg === "-t" || $arg === "--index-items"){
                if(self::$items){
                    throw new errException(1);
                }
                //TODO START DEPENDENCY
                self::$items = TRUE;
            }

            elseif(substr($arg, 0, 8) === "--start="){

                self::$dependency = TRUE;

                if(self::$start !== 1){
                    throw new errException(1);
                    //TODO START 1
                }
                else{
                    self::$start = substr($arg, 8);
                    // Dependency index-items and start
                    if (!(preg_match('!^[1-9][0-9]*$!', self::$start))) {
                        throw new errException(1);
                    }
                }
            }

            else{
                // Unknown argument, unable to process
                throw new errException(1);
            }

            // Help single arg. check
            if(count($argv) > 1 && self::$help) {
                throw new errException(1);
            }

            //Start indexing default value check
            if(self::$items && self::$start === NULL){
                self::$start = 1;
            }
        }

        if(self::$dependency && !(self::$items)){
            throw new errException(1);
        }
    }

}

class xml {
    private
        $data = NULL,
        $output = NULL,
        $element = NULL,
        $attribute = NULL;


    public function xml_encoder($json){
        foreach ($json as $data){
            echo $data;
            $output = $data."\n";

        }
    }
}


// Class for handling errors (errorException is not fitting)
class errException extends Exception {
    public $code, $message;

    public function __construct($code){
        $this->code = $code;
    }

    public function __toString(){
        if($this->code == 1){
            $this->message = "špatný formát parametrů skriptu nebo byla použita zakázaná kombinace parametrů";
        }
        elseif($this->code == 2){
            $this->message = "neexistující zadaný vstupní soubor nebo chyba otevření zadaného vstupního souboru pro čtení";
        }
        elseif($this->code == 3){
            $this->message = "chyba při pokusu o otevření zadaného výstupního souboru pro zápis";
        }
        elseif($this->code == 4){
            $this->message = "chybný formát vstupního souboru";
        }
        //TODO

        return "Error: ".$this->message." (error code: ".$this->code.")\n";
    }
}

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
}


?>