/*********************************************
 *   Program :   str.h
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
 *   Notes   :   Definition of string functions
 ********************************************/

#ifndef STR
#define STR

#define STR_ALLOC_SIZE  256

typedef struct string {
    unsigned len;
    unsigned max;
    char *str;
} string_t;

typedef struct str_list {
    char *str;
    struct str_list *next;
} str_list_item_t;

typedef struct {
    str_list_item_t *first;
    str_list_item_t *last;
} str_list_t;


string_t str_init();
void str_reinit(string_t *str);
void str_destroy(string_t *str);
void str_print(string_t str);
void str_resize(string_t *str);
void str_append_str(string_t *str_dest,string_t *str_src);

int str_find(string_t *str,string_t *search);

void str_sort(string_t *str);
string_t str_sub_str(string_t str, int i, int n);

void str_concatenate(string_t *str_dest, string_t *str_src1, string_t *str_src2);
void str_append_chars(string_t *str_dest,char *str_src);
void str_read_str_stdin(string_t* dest_str);

int str_cmp(string_t str1, string_t str2);

//makro pre pridanie znaku do stringu
#define ADD_CHAR(string,c)  {if (!(string.len < string.max)) str_resize(&string);\
                             string.str[string.len++] = c;   }

//inline funkcie pre pridanie znaku, asi lepsia alternativa ako makro
static inline void str_add_char(string_t *string, char c){
    if (string->len >= string->max) {
        str_resize(string);
    }
    string->str[string->len++] = c;
}



#endif  //STR
