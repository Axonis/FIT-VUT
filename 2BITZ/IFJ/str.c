/*********************************************
 *   Program :   str.c
 *   Authors :   Roman Dobiáš     - xdobia11
 *               Adrián Tomašov   - xtomas32
 *               Jozef Urbanovský - xurban66
 *               Adam Šulc        - xsulca00
 *               Kristián Barna   - xbarna02
 *   Skupina :   2BIB(2016)
 *   Created :   01.10.2016
 *   Compiled:   gcc 4.9.2
 *   Project :   IFJ16
 *
 *   Notes   :   Implementation of string functions
 ********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "str.h"
#include "ial.h"
#include "error.h"

string_t str_init(){
    string_t new_str;

    new_str.str = calloc(STR_ALLOC_SIZE + 1,sizeof(char));
    if (new_str.str == NULL){
        error_and_die(INTERNAL_ERROR, "Calloc failed in string module");
    }
    new_str.len = 0;
    new_str.max = STR_ALLOC_SIZE;

    return new_str;
}

void str_reinit(string_t *str){
    if (str->str != NULL) {
        free(str->str);
    }

    str->max = STR_ALLOC_SIZE;
    str->len = 0;

    if ((str->str = calloc(STR_ALLOC_SIZE + 1,sizeof(char))) == NULL) {
        error_and_die(INTERNAL_ERROR, "Calloc failed in string module");
    }
}
void str_print(string_t str){
    printf("%s\n",str.str);
    printf("len: %d\n",str.len);
}
void str_resize(string_t *str) {
    char *new_str;
    str->max += STR_ALLOC_SIZE;
    if ((new_str = calloc(str->max + 1, sizeof(char))) == NULL){
        error_and_die(INTERNAL_ERROR, "Calloc failed in string module");
    }
    strcpy(new_str,str->str);
    free(str->str);
    str->str = new_str;
}
void str_append_chars(string_t *str_dest,char *str_src){
    int i = 0;

    while (str_src[i] != '\0'){
        if (str_dest->len >= str_dest->max) {
            str_resize(str_dest);
        }
        str_dest->str[str_dest->len++] = str_src[i++];
    }
}
void str_append_str(string_t *str_dest,string_t *str_src){
    while ((str_dest->len + str_src->len) > str_dest->max) {
        str_resize(str_dest);
    }

    for (int i = 0; i < str_src->len; i++){
        str_dest->str[str_dest->len++] = str_src->str[i];
    }

}


int str_find(string_t *str,string_t *search) {
    return KMP_hladaj(str->str,search->str);
}

void str_sort(string_t *str) {
    ial_qsort(str->str, 0, str->len - 1);
}

string_t str_sub_str(string_t str, int i, int n) {
    int end = i + n;
    //pocet ostavajucich znakov musi byt vacsi ako pocet znakov na odstrihnnutie
    if ((str.len ) < end){
        error_and_die(INTERNAL_ERROR, "Substring error");
    }

    string_t ret = str_init();

    for (int j = i; j < end ; j++){
        str_add_char(&ret,str.str[j]);
    }

    return ret;
}

void str_concatenate(string_t *str_dest, string_t *str_src1, string_t *str_src2) {
    if (str_dest == str_src1 || str_dest == str_src2) {
        string_t tmp_str = str_init();          //pomocny string kde sa ulozi predosly vysledok
        str_append_str(&tmp_str, str_dest);
        str_concatenate(str_dest, &tmp_str, &tmp_str);    //rekurzivne volanie na konkatenaciu stringov
        str_destroy(&tmp_str);
    } else {
        int len = str_src1->len + str_src2->len;  //velkost vysledneho stringu
        int len_first = str_src1->len;
        int i, j;

        str_reinit(str_dest);

        //zvacsenie stringu aby mohol pohltit oba stringy
        while (len > str_dest->max) {
            str_resize(str_dest);
        }

        for (i = 0; i < len_first; i++) {
            str_dest->str[str_dest->len++] = str_src1->str[i];
        }
        for (j = 0; i < len; i++, j++) {
            str_dest->str[str_dest->len++] = str_src2->str[j];
        }
    }
}
void str_read_str_stdin(string_t* dest_str){
    int c;
    str_reinit(dest_str);
    while ((c = fgetc(stdin)) != EOF && c != '\n'){
       str_add_char(dest_str,(char)c);
    }
}

int str_cmp(string_t str1, string_t str2) {
    int ret = strcmp(str1.str, str2.str);
    if (!ret) {
        return 0;
    } else {
        if (ret > 0) {
            return 1;
        } else {
            return -1;
        }
    }
}
void str_destroy(string_t *str) {
	if(str->str)
	    free(str->str);
	str->str = NULL;
}
