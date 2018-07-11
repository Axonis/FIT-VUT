var tmp = [];
function removeItem(array, item){
    for(var i in array){
        if(array[i] == item){
            array.splice(i,1);
            break;
        }
    }
}

function reply_click(clicked) {
    if($(clicked).hasClass('active')) {
        removeItem(tmp, clicked.id);
        $(clicked).removeClass('active');
    }
    else {
        tmp.push(clicked.id);
        $(clicked).addClass('active');
    }
}

function convert() {
    $('#id_bug').attr('value', tmp.toString());
}

function single_click(clicked) {
    $('tr').removeClass('active');
    $(clicked).addClass('active');
    $('#id_patch').attr('value',clicked.id);

}

function copy() {
    $("#bug_form").clone(true).appendTo("#append_point");
}