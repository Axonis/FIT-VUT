/**
 * Created by k on 26.11.2016.
 */

var login_state = -1;
//var login_state = 1;
var meno_arr = new Array("admin", "a", "z");
var priez_arr = new Array("admin", "A", "Z");
var heslo_arr = new Array("admin", "2", "3");
var counter = 0;
var pom = 0;

function loginEnable(){
    pom = 1;
    document.getElementById("fb_id").style.display = "none";
    document.getElementById("login_id").style.display="none";
    document.getElementById("login_msg_id").style.display="block";
    document.getElementById("status_id").innerHTML = "Prihlásený: "+meno_arr[login_state];
    document.getElementById("acc_header").innerHTML = "Vitajte,  "+meno_arr[login_state]+".";
}

function loginDisable(){
    document.getElementById("login_id").classList.add("animate");

    document.getElementById("login_id").style.display="block";
    document.getElementById("login_msg_id").style.display="none";

    document.getElementById("status_id").innerHTML = "Správa užívateľov";
}

function statsEnable() {
    document.getElementById("stats_btn_id").setAttribute("href", "#stats-modal");
}

function statsDisable() {
    document.getElementById("stats_btn_id").setAttribute("href", "#stats-modal_logout");
}

function logout(){
    login_state = -1;
    loginDisable();
}

function deleteAcc() {

    if (meno_arr[login_state] != meno_arr[meno_arr.length - 1])
        meno_arr[login_state] = meno_arr[meno_arr.length - 1];
    meno_arr.splice(meno_arr.length - 1, 1);

    if (heslo_arr[login_state] != heslo_arr[heslo_arr.length -1])
        heslo_arr[login_state] = heslo_arr[heslo_arr.length -1];
    heslo_arr.splice(heslo_arr.length - 1, 1);

    if (priez_arr[login_state] != priez_arr[priez_arr.length -1])
        priez_arr[login_state] = priez_arr[priez_arr.length -1];
    priez_arr.splice(priez_arr.length - 1, 1);

    login_state = -1;

    loginDisable();
}

function validateForm() {
    var meno = document.login_form.meno.value;
    var heslo = document.login_form.heslo.value;
    var modal = document.getElementById('fb_id');
    var i;
    //ak sa prihlasuje cez normalne
    if (modal.style.display != "block") {
        for (i = 0; i < meno_arr.length; i++) {
            if ((meno == meno_arr[i]) && (heslo == heslo_arr[i])) {
                login_state = i;
                loginEnable();
                return;
            }
        }
        //ak sa meno / heslo nenaslo v poli
        document.getElementById("login_id").classList.add("wrong_login");
        setTimeout(function () {
            document.getElementById("login_id").classList.remove("wrong_login", "animate");
        }, 300);
        return;
    }
    //ak sa prihlasuje cez fb
    else {
        meno = document.login_fb_form.meno.value;
        heslo = document.login_fb_form.heslo.value;

        for (i = 0; i < meno_arr.length; i++) {
            if ((meno == meno_arr[i]) && (heslo == heslo_arr[i])) {
                login_state = i;
                loginEnable();
                return;
            }
        }
        //ak sa meno / heslo nenaslo v poli
        document.getElementById("fb_id").classList.add("wrong_login");
        setTimeout(function () {
            document.getElementById("fb_id").classList.remove("wrong_login", "animate");
        }, 300);
        return;
    }
}

function regWronk() {
    document.getElementById("reg_login_id").classList.add("wrong_login");
    setTimeout(function () {
        document.getElementById("reg_login_id").classList.remove("wrong_login", "animate");
    }, 300);
}

function registration() {

    var meno = document.registracia.meno.value;
    var priez = document.registracia.priez.value;
    var heslo = document.registracia.heslo.value;
    var heslo_rep = document.registracia.heslo_rep.value;
    var email = document.registracia.email.value;

    if (!meno || !priez || !heslo){
        document.getElementById("login_sprava_reg").innerHTML = "Prosím vyplňte povinné údaje (*).";
        regWronk();
        return;
    }

    for (var i = 0; i < meno_arr.length; i++){
        if (meno == meno_arr[i]){
            document.getElementById("login_sprava_reg").innerHTML = "Toto meno je už obsadené.";
            regWronk();
            return;
        }
    }

    if (heslo == heslo_rep) {
        meno_arr[meno_arr.length] = meno;
        priez_arr[priez_arr.length] = priez;
        heslo_arr[heslo_arr.length] = heslo;
    }
    else{
        document.getElementById("login_sprava_reg").innerHTML = "Heslá sa nezhodujú.";
        regWronk();
        return;
    }


    document.getElementById("login_sprava_reg").innerHTML = "Vitajte "+meno+"! Úspešne ste sa registrovali.";

    return;

}

function slider_fade_in() {
    $('#slider_id').animate({
        opacity: 1
    }, 500);
}

function eventFire(el, etype){
    if (el.fireEvent) {
        el.fireEvent('on' + etype);
    } else {
        var evObj = document.createEvent('Events');
        evObj.initEvent(etype, true, false);
        el.dispatchEvent(evObj);
    }
}

function close() {
    var modal_1 = document.getElementById("fb_id");

    // ak pouzivatel klikne hocikde prec z modalu tak sa zavre
    window.onclick = function (event) {
        //pre facebook login
        if (event.target == modal_1) {
            modal_1.style.display = "none";
            document.getElementById("login_id").classList.add("animate");
            document.getElementById('login_id').style.display="block";
        }
        else {
            //pre registraciu
            if (login_state == -1 && document.getElementById('reg_id').style.display == "block"
                && !event.target.classList.contains('script_pom_3') && event.target.id != "reg_btn_id"){
                eventFire(document.getElementById('exit_btn_id'), 'click');
            }
            else {
                //pre login okno
                if (login_state != -1) {
                    //script_pom_2 classa butonu na hlavnej stranke na zobrazenie prihlasovania
                    if (pom == 1 && event.target.classList.contains('script_pom_2')) {
                        pom = 2;
                    }
                    else {
                        //script_pom classa obsahu okna po prihlaseni
                        if (pom == 2 && !event.target.classList.contains('script_pom')
                        ) {
                            document.getElementById("login_msg_id").style.display = "none";
                            pom = 0;
                            eventFire(document.getElementById('white_cross'), 'click');
                        }
                    }
                }
            }
        }
    }
}

function removeLogin(){
    $('#slider_id').animate({
        opacity: 0
    }, 500);

    if (login_state != -1){
        loginEnable()
    }
    else{
        loginDisable()
    }
    pom = 1;
}

function statsValidate() {
    if (login_state != -1){
        statsEnable();
        progress_anim_nuluj();
        progress_anim();
    }
    else{
        statsDisable();
    }
}

function progress_anim() {
    $('#bar1').delay('200').animate({
        width: '60%'
    }, 200);
    $('#bar2').delay('200').animate({
        width: '95%'
    }, 200);
    $('#bar3').delay('200').animate({
        width: '80%'
    }, 200);
    $('#bar4').delay('200').animate({
        width: '100%'
    }, 200);
}

function progress_anim_nuluj() {
    document.getElementById('bar1').style.width="0";
    document.getElementById('bar2').style.width="0";
    document.getElementById('bar3').style.width="0";
    document.getElementById('bar4').style.width="0";
}

function koment(pom){
    var koment_1 = document.getElementById("koment_1");
    var koment_2 = document.getElementById("koment_2");
    var koment_3 = document.getElementById("koment_3");

    switch(pom){
        case 0 :
            koment_1.style.display = "block";
            koment_2.style.display = "none";
            koment_3.style.display = "none";
            setTimeout(function() {
                document.getElementById("stats_progress").style.border = " solid 2px #28ABE3";
            }, 500);

            break;
        case 1 :
            koment_1.style.display = "none";
            koment_2.style.display = "block";
            koment_3.style.display = "none";
            setTimeout(function() {
                document.getElementById("stats_progress").style.border = " solid 2px #72bf48";
            }, 500);
            break;
        case 2 :
            koment_1.style.display = "none";
            koment_2.style.display = "none";
            koment_3.style.display = "block";
            setTimeout(function() {
                document.getElementById("stats_progress").style.border = " solid 2px #FF432E";
            }, 500);
            break;
    }
}

function koment_next(){
    counter = (counter + 1) % 3 ;
    koment(counter);
}

function koment_prew() {
    counter = (counter - 1) % 3;
    if (counter < 0)
        counter = 2;
    koment(counter);


}