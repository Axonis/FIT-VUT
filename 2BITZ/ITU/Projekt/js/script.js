var $ = jQuery.noConflict();



// Progress Bar

$(document).ready(function ($) {
    "use strict";

    $('.skill-shortcode').appear(function () {
        $('.progress').each(function () {
            $('.progress-bar').css('width',  function () { return ($(this).attr('data-percentage') + '%')});
        });
    }, {accY: -100});

    $(".close-modal").mouseenter(function () {
        $(".lr").css("background-color", "red").css("width", "2");
        $(".rl").css("background-color", "red").css("width", "2");
    });

    $(".close-modal").mouseleave(function () {
        $(".lr").css("background-color", "#999").css("width", "1");;
        $(".rl").css("background-color", "#999").css("width", "1");;
    });

    $("#test-modal .number").click(function(){
        $(".question").hide("slow");
        $(".test").show("slow");
    });

    $("#exercise-modal .number").click(function(){
        $(".question").hide("slow");
        $(".exercise").show("slow");
    });


    $('#q1 label').click(function(){
         $(".exercise .right1").addClass("btn-success").removeClass("btn-primary");
         $(".exercise .wrong1").addClass("btn-danger").removeClass("btn-primary");
         $("#r1 .glyphicon-menu-right").show("slow");
    });

    $('#r1 .glyphicon-menu-right').click(function(){
        $(".one").removeClass("active");
        $(".two").addClass("active");
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#q2 label').click(function(){
        $(".exercise .right2").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong2").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
    });

    $('#r2 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".two").removeClass("active");
        $(".three").addClass("active");
        $('#r3 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l2 .glyphicon-menu-left').click(function(){
        $(".two").removeClass("active");
        $(".one").addClass("active");
    });

    $('#q3 label').click(function(){
        $(".exercise .right3").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong3").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r3 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".three").removeClass("active");
        $(".four").addClass("active");
        $('#r4 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l3 .glyphicon-menu-left').click(function(){
        $(".three").removeClass("active");
        $(".two").addClass("active");
    });

    $('#q4 label').click(function(){
        $(".exercise .right4").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong4").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r4 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".four").removeClass("active");
        $(".five").addClass("active");
        $('#r5 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l4 .glyphicon-menu-left').click(function(){
        $(".four").removeClass("active");
        $(".three").addClass("active");
    });

    $('#q5 label').click(function(){
        $(".exercise .right5").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong5").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r5 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".five").removeClass("active");
        $(".six").addClass("active");
        $('#r6 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l5 .glyphicon-menu-left').click(function(){
        $(".five").removeClass("active");
        $(".four").addClass("active");
    });

    $('#q6 label').click(function(){
        $(".exercise .right6").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong6").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r6 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".six").removeClass("active");
        $(".seven").addClass("active");
        $('#r7 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l6 .glyphicon-menu-left').click(function(){
        $(".six").removeClass("active");
        $(".five").addClass("active");
    });

    $('#q7 label').click(function(){
        $(".exercise .right7").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong7").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r7 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".seven").removeClass("active");
        $(".eight").addClass("active");
        $('#r8 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l7 .glyphicon-menu-left').click(function(){
        $(".seven").removeClass("active");
        $(".six").addClass("active");
    });

    $('#q8 label').click(function(){
        $(".exercise .right8").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong8").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r8 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".eight").removeClass("active");
        $(".nine").addClass("active");
        $('#r9 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l8 .glyphicon-menu-left').click(function(){
        $(".eight").removeClass("active");
        $(".seven").addClass("active");
    });

    $('#q9 label').click(function(){
        $(".exercise .right9").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong9").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r9 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".nine").removeClass("active");
        $(".ten").addClass("active");
        $('#r10 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l9 .glyphicon-menu-left').click(function(){
        $(".nine").removeClass("active");
        $(".eight").addClass("active");
    });

    $('#q10 label').click(function(){
        $(".exercise .right10").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong10").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r10 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".ten").removeClass("active");
        $(".eleven").addClass("active");
        $('#r11 .glyphicon-menu-right').hide();
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l10 .glyphicon-menu-left').click(function(){
        $(".ten").removeClass("active");
        $(".nine").addClass("active");
    });

    $('#q11 label').click(function(){
        $(".exercise .right11").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong11").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-right").show("slow");
        $(".glyphicon-menu-left").show("slow");
    });

    $('#r11 .glyphicon-menu-right').click(function(){
        $(".glyphicon-menu-left").hide();
        $(".eleven").removeClass("active");
        $(".twelve").addClass("active");
        $(".glyphicon-menu-left").delay(1000).show("slow");
    });

    $('#l11 .glyphicon-menu-left').click(function(){
        $(".eleven").removeClass("active");
        $(".ten").addClass("active");
    });

    $('#q12 label').click(function(){
        $(".exercise .right12").addClass("btn-success").removeClass("btn-primary");
        $(".exercise .wrong12").addClass("btn-danger").removeClass("btn-primary");
        $(".glyphicon-menu-left").show("slow");
        $(".exercise-vyhodnotit").show("slow");
    });

    $('#l12 .glyphicon-menu-left').click(function(){
        $(".twelve").removeClass("active");
        $(".eleven").addClass("active");
    });

    $(".exercise-vyhodnotit").click(function() {
        $(".exercise-footer").hide();

        var spravne = 0;

        for (var i = 1; i <= 12; i++) {
            if (!$("#q"+i+" label").hasClass("active")) {
                $("#results").append("Otázka "+i+": -<br>");
            }

            else if ($("#t"+i+" label").hasClass("right"+i+" active")) {
                $("#results").append("Otázka "+i+": Správne<br>");
                spravne += 1;
            }
            else
                $("#results").append("Otázka "+i+": Nesprávne<br>");
        }
        $("#results").append("Otázok správne: "+spravne+"<br>");


    });



    $('#t1 label').click(function(){
        setTimeout(function(){
            $("#t1").removeClass("active in");
            $("#t2").addClass("active in");
            $(".tone").removeClass("active");
            $(".ttwo").addClass("active");
        },250);
    });

    $('#t2 label').click(function(){
        setTimeout(function(){
            $("#t2").removeClass("active in");
            $("#t3").addClass("active in");
            $(".ttwo").removeClass("active");
            $(".tthree").addClass("active");
        },250);
    });

    $('#t3 label').click(function(){
        setTimeout(function(){
            $("#t3").removeClass("active in");
            $("#t4").addClass("active in");
            $(".tthree").removeClass("active");
            $(".tfour").addClass("active");
        },250);
    });

    $('#t4 label').click(function(){
        setTimeout(function(){
            $("#t4").removeClass("active in");
            $("#t5").addClass("active in");
            $(".tfour").removeClass("active");
            $(".tfive").addClass("active");
        },250);
    });

    $('#t5 label').click(function(){
        setTimeout(function(){
            $("#t5").removeClass("active in");
            $("#t6").addClass("active in");
            $(".tfive").removeClass("active");
            $(".tsix").addClass("active");
        },250);
    });

    $('#t6 label').click(function(){
        setTimeout(function(){
            $("#t6").removeClass("active in");
            $("#t7").addClass("active in");
            $(".tsix").removeClass("active");
            $(".tseven").addClass("active");
        },250);
    });

    $('#t7 label').click(function(){
        setTimeout(function(){
            $("#t7").removeClass("active in");
            $("#t8").addClass("active in");
            $(".tseven").removeClass("active");
            $(".teight").addClass("active");
        },250);
    });

    $('#t8 label').click(function(){
        setTimeout(function(){
            $("#t8").removeClass("active in");
            $("#t9").addClass("active in");
            $(".teight").removeClass("active");
            $(".tnine").addClass("active");
        },250);
    });

    $('#t9 label').click(function(){
        setTimeout(function(){
            $("#t9").removeClass("active in");
            $("#t10").addClass("active in");
            $(".tnine").removeClass("active");
            $(".tten").addClass("active");
        },250);
    });

    $('#t10 label').click(function(){
        setTimeout(function(){
            $("#t10").removeClass("active in");
            $("#t11").addClass("active in");
            $(".tten").removeClass("active");
            $(".televen").addClass("active");
        },250);
    });

    $('#t11 label').click(function(){
        setTimeout(function(){
            $("#t11").removeClass("active in");
            $("#t12").addClass("active in");
            $(".televen").removeClass("active");
            $(".ttwelve").addClass("active");
        },250);
    });

    $('#t12 label').click(function(){
        setTimeout(function(){
            $(".test-vyhodnotit").show("slow").addClass("btn-info").removeClass("btn-default");
        },250);
    });

    $(".test-vyhodnotit").click(function() {
        $(".test-footer").hide();

        var spravne = 0;

        for (var i = 1; i <= 12; i++) {
            if (!$("#t"+i+" label").hasClass("active")) {
                $("#tresults").append("Otázka "+i+": -<br>");
            }

            else if ($("#t"+i+" label").hasClass("right"+i+" active")) {
                $("#tresults").append("Otázka "+i+": Správne<br>");
                spravne += 1;
            }
            else
                $("#tresults").append("Otázka "+i+": Nesprávne<br>");
        }
        $("#tresults").append("Otázok správne: "+spravne+"<br>");


    });

    var spravne_signs = 0;
    var uspesnost = 0;
    var i = 0;

    $('.light-orange').click(function(){
        spravne_signs = 0;
        uspesnost = 0;
        i = 0;
    });

    $('#s1 label').click(function(){
        $(".signs .right1").addClass("btn-success").removeClass("btn-primary");
        $(".signs .wrong1").addClass("btn-danger").removeClass("btn-primary");
        $("#sr1 .glyphicon-menu-right").show("slow");
    });

    $('.right1').one("click", function () {
        i=1;
        spravne_signs += 1;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " + String(uspesnost)+" %";
    });

    $('.wrong1').one("click", function () {
        i=1;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " + String(uspesnost)+" %";
    });

    $('#s2 label').click(function(){
        $(".signs .right2").addClass("btn-success").removeClass("btn-primary");
        $(".signs .wrong2").addClass("btn-danger").removeClass("btn-primary");
        $("#sr2 .glyphicon-menu-right").show("slow");
    });

    $('.right2').one("click", function () {
        i=2;
        spravne_signs +=1;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " +String(uspesnost)+" %";
    });

    $('.wrong2').one("click", function () {
        i=2;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " +String(uspesnost)+" %";
    });

    $('#s3 label').click(function(){
        $(".signs .right3").addClass("btn-success").removeClass("btn-primary");
        $(".signs .wrong3").addClass("btn-danger").removeClass("btn-primary");
        $("#sr3 .glyphicon-menu-right").show("slow");
    });

    $('.right3').one("click", function () {
        i=3;
        spravne_signs +=1;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " +String(uspesnost)+" %";
    });

    $('.wrong3').one("click", function () {
        i=3;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " +String(uspesnost)+" %";
    });


    $('#s4 label').click(function(){
        $(".signs .right4").addClass("btn-success").removeClass("btn-primary");
        $(".signs .wrong4").addClass("btn-danger").removeClass("btn-primary");
        $("#sr4 .glyphicon-menu-right").show("slow");
    });

    $('.right4').one("click", function () {
        i=4;
        spravne_signs +=1;
        uspesnost = spravne_signs / i * 100;
        document.getElementById("sresults").innerHTML = "Úspešnosť: " +String(uspesnost)+" %";
    });

    $('.wrong4').one("click", function () {
        i=4;
        uspesnost = spravne_signs / i * 100;
        uspesnost = parseInt(uspesnost, 10);
        document.getElementById("sresults").innerHTML = "Úspešnosť: " + String(uspesnost)+" %";
    });


    var spravne_cross = 0;
    var cuspesnost = 0;
    var j = 0;
    $('.azure').click(function(){
        spravne_cross = 0;
        cuspesnost = 0;
        j = 0;
    });

    $('#c1 label').click(function(){
        $(".crossroads .right1").addClass("btn-success").removeClass("btn-primary");
        $(".crossroads .wrong1").addClass("btn-danger").removeClass("btn-primary");
        $("#cr1 .glyphicon-menu-right").show("slow");
    });

    $('.right1').one("click", function () {
        j=1;
        spravne_cross += 1;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('.wrong1').one("click", function () {
        j=1;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('#c2 label').click(function(){
        $(".crossroads .right2").addClass("btn-success").removeClass("btn-primary");
        $(".crossroads .wrong2").addClass("btn-danger").removeClass("btn-primary");
        $("#cr2 .glyphicon-menu-right").show("slow");
    });

    $('.right2').one("click", function () {
        j=2;
        spravne_cross += 1;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('.wrong2').one("click", function () {
        j=2;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('#c3 label').click(function(){
        $(".crossroads .right3").addClass("btn-success").removeClass("btn-primary");
        $(".crossroads .wrong3").addClass("btn-danger").removeClass("btn-primary");
        $("#cr3 .glyphicon-menu-right").show("slow");
    });

    $('.right3').one("click", function () {
        j=3;
        spravne_cross += 1;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('.wrong3').one("click", function () {
        j=3;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });


    $('#c4 label').click(function(){
        $(".crossroads .right4").addClass("btn-success").removeClass("btn-primary");
        $(".crossroads .wrong4").addClass("btn-danger").removeClass("btn-primary");
        $("#cr4 .glyphicon-menu-right").show("slow");
    });

    $('.right4').one("click", function () {
        j=4;
        spravne_cross += 1;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    $('.wrong4').one("click", function () {
        j=4;
        cuspesnost = spravne_cross / i * 100;
        cuspesnost = parseInt(cuspesnost, 10);
        document.getElementById("cresults").innerHTML = "Úspešnosť: " + String(cuspesnost)+" %";
    });

    var trigger = $('.hamburger'),
        isClosed = true;

    trigger.click(function () {
        hamburger_cross();
    });

    function hamburger_cross() {

        if (isClosed == true) {
            trigger.removeClass('is-open');
            trigger.addClass('is-closed');
            isClosed = false;
        } else {
            trigger.removeClass('is-closed');
            trigger.addClass('is-open');
            isClosed = true;
        }
    }

    $('[data-toggle="offcanvas"]').click(function () {
        $('#wrapper').toggleClass('toggled');
    });

    $("#u1").click(function () {
        $("#ucebne1").show("slow");
        $("#ucebne2").hide("slow");
        $("#u1").addClass("blue");
        $("#u2").removeClass("green");
    });

    $("#u2").click(function () {
        $("#ucebne2").show("slow");
        $("#ucebne1").hide("slow");
        $("#u2").addClass("green");
        $("#u1").removeClass("blue");
    });



});