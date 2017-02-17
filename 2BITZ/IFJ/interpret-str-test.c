#include <stdio.h>
#include "ial.h"
#include "interpret.h"
#include "instruction_list.h"

int main() {
    stab_t *sym_tab;
    data_t tmp_var;
    instruction_list_t *i_list;

    char str1[] = "qwertyuiop";

    data_t *ptr_to_table1, *ptr_to_table2, *ptr_to_table3, *ptr_to_table4;


    printf("Test nad string operaciami v globalnych premennych\n"
                   "v str1 je ulozeny retazec\n"
                   "do str2 sa pomocou str2 nahra retazec zo stdin\n"
                   "do str3 sa ulozi ich konkatenacia\n"
                   "do int a sa ulozi dlzka str2\n"
                   "do int b sa ulozi dlzka str3\n\n");

    sym_tab = stable_init(1024);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, str1);
    stable_add_var(sym_tab, "glob_str_1", tmp_var);
    tmp_var.data.data.s = str_init();
    stable_add_var(sym_tab, "glob_str_2", tmp_var);
    tmp_var.data.data.s = str_init();
    stable_add_var(sym_tab, "glob_str_3", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    stable_add_var(sym_tab, "INT.A", tmp_var);
    stable_add_var(sym_tab, "INT.B", tmp_var);

    //stable_print(sym_tab);

    i_list = init_inst_list();

    ptr_to_table1 = stable_get_var(sym_tab, "glob_str_1");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "glob_str_2");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "glob_str_2");
    create_and_add_instruction(i_list, INST_READ_STRING, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "glob_str_2");
    ptr_to_table2 = stable_get_var(sym_tab, "INT.A");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_STR_LEN, &ptr_to_table2->data, &ptr_to_table1->data, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table2->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "glob_str_1");
    ptr_to_table2 = stable_get_var(sym_tab, "glob_str_2");
    ptr_to_table3 = stable_get_var(sym_tab, "glob_str_3");

    create_and_add_instruction(i_list, INST_STR_CONCATENATE, &ptr_to_table3->data, &ptr_to_table2->data,
                               &ptr_to_table1->data);

    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);
    ptr_to_table2 = stable_get_var(sym_tab, "INT.B");
    create_and_add_instruction(i_list, INST_STR_LEN, &ptr_to_table2->data, &ptr_to_table3->data, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table2->data, 0, 0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    //stable_print(sym_tab);

    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\n"
                   "TEST2\n"
                   "praca so stringami na zasobniku\n"
                   "vytvorime 3 lokalne stringy {str1,str2,str3}\n"
                   "do prvych 2 nacitame hodnotu pomocou read_string\n"
                   "a do str3 ulozime ich konkatenaciu\n\n"
                   "Umiestnenie na stacku:\n"
                   "-----------------------------\n"
                   "|str1                      1|\n"
                   "|str2                      2|\n"
                   "|str3                      3|\n"
                   "|len1                      4|\n"
                   "|len2                      5|\n"
                   "|l2n3                      6|\n"
                   "-----------------------------\n"
    );

    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str_1", tmp_var);
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str_2", tmp_var);
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "str_3", tmp_var);
    tmp_var.data.data.i = 4;
    stable_add_var(sym_tab, "len_1", tmp_var);
    tmp_var.data.data.i = 5;
    stable_add_var(sym_tab, "len_2", tmp_var);
    tmp_var.data.data.i = 6;
    stable_add_var(sym_tab, "len_3", tmp_var);

    //vytvorenie lokalnych premennych
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str_1");
    ptr_to_table2 = stable_get_var(sym_tab, "str_2");
    ptr_to_table3 = stable_get_var(sym_tab, "len_1");

    create_and_add_instruction(i_list, INST_READ_STRING, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_STR_LEN, &ptr_to_table3->data, &ptr_to_table1->data, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);

    ptr_to_table3 = stable_get_var(sym_tab, "len_2");
    create_and_add_instruction(i_list, INST_READ_STRING, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_STR_LEN, &ptr_to_table3->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str_1");
    ptr_to_table2 = stable_get_var(sym_tab, "str_2");
    ptr_to_table3 = stable_get_var(sym_tab, "str_3");
    create_and_add_instruction(i_list, INST_STR_CONCATENATE, &ptr_to_table3->data, &ptr_to_table1->data,
                               &ptr_to_table2->data);
    ptr_to_table2 = stable_get_var(sym_tab, "len_3");
    create_and_add_instruction(i_list, INST_STR_LEN, &ptr_to_table2->data, &ptr_to_table3->data, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table2->data, 0, 0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);

    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\nTEST 3\n"
                   "");

    i_list = init_inst_list();
    sym_tab = stable_init(10);


    tmp_var.data.arg_type = STACK_EBP;

    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "INT.A", tmp_var);
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str_1", tmp_var);
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "str_2", tmp_var);

    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str_1");
    ptr_to_table2 = stable_get_var(sym_tab, "str_2");
    ptr_to_table3 = stable_get_var(sym_tab, "INT.A");
    create_and_add_instruction(i_list, INST_READ_STRING, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_READ_STRING, &ptr_to_table2->data, 0, 0);

    create_and_add_instruction(i_list, INST_STR_CMP, &ptr_to_table3->data, &ptr_to_table2->data, &ptr_to_table1->data);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);


    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    i_list = init_inst_list();
    sym_tab = stable_init(10);

    printf("\n\nGLOBAL ONLY"
                   "\nTEST4\nvolanie funkcii pracujucich nad stringami a printom\n"
                   "pushne 4 stringy a zavola print ocakavany vystup\n"
                   "abc def 123 !@#\n");

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "abc ");
    stable_add_var(sym_tab, "str1", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "def ");
    stable_add_var(sym_tab, "str2", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "123 ");
    stable_add_var(sym_tab, "str3", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "!@# ");
    stable_add_var(sym_tab, "str4", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "\n");
    stable_add_var(sym_tab, "new_line", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 5;
    stable_add_var(sym_tab, "num_of_push", tmp_var);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str2");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str3");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str4");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "new_line");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "num_of_push");
    create_and_add_instruction(i_list, INST_CALL_PRINT, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\nTEST5\n"
                   "zavola funkciu CALL_STR_CMP a porovnava stringy\n"
                   "ocakavany vystup\n"
                   "0\n"
                   "1\n\n");

    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "abc");
    stable_add_var(sym_tab, "str1", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "abc");
    stable_add_var(sym_tab, "str2", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "123 ");
    stable_add_var(sym_tab, "str3", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    stable_add_var(sym_tab, "ret", tmp_var);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str2");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "ret");
    create_and_add_instruction(i_list, INST_CALL_CMP, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str3");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "ret");
    create_and_add_instruction(i_list, INST_CALL_CMP, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST6\n"
                   "podobny test ako test4 a test5 ale s premennymi na zasobniku\n");

    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str1", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str2", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "str3", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 4;
    stable_add_var(sym_tab, "n", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 5;
    stable_add_var(sym_tab, "a", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "abc");
    stable_add_var(sym_tab, "abc", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "#!@");
    stable_add_var(sym_tab, "#!@", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "\n");
    stable_add_var(sym_tab, "\n", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "num_args", tmp_var);
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "new_line", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "abc");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str2");
    ptr_to_table2 = stable_get_var(sym_tab, "abc");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "str3");
    ptr_to_table2 = stable_get_var(sym_tab, "#!@");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    ptr_to_table1 = stable_get_var(sym_tab, "n");
    ptr_to_table2 = stable_get_var(sym_tab, "num_args");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "str2");
    ptr_to_table3 = stable_get_var(sym_tab, "str3");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "n");
    create_and_add_instruction(i_list, INST_CALL_PRINT, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "\n");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "str2");
    ptr_to_table3 = stable_get_var(sym_tab, "a");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_CMP, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);


    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "str3");
    ptr_to_table3 = stable_get_var(sym_tab, "a");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_CMP, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\n"
                   "TEST7\n"
                   "volanie strfind nad globalnymi premennymi\n");


    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"qwertyuiopasdfghjkl");
    str_append_chars(&tmp_var.data.data.s, str1);
    stable_add_var(sym_tab, "glob_str_1", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"opasdfgh");
    stable_add_var(sym_tab, "glob_str_2", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"bnmkl");
    stable_add_var(sym_tab, "glob_str_3", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"");
    stable_add_var(sym_tab, "glob_str_4", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    stable_add_var(sym_tab, "ret", tmp_var);

    ptr_to_table1 = stable_get_var(sym_tab,"glob_str_1");
    ptr_to_table2 = stable_get_var(sym_tab,"glob_str_2");
    ptr_to_table3 = stable_get_var(sym_tab,"glob_str_3");
    ptr_to_table4 = stable_get_var(sym_tab,"ret");

    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table2->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_FIND,&ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table4->data,0,0);


    ptr_to_table2 = stable_get_var(sym_tab,"glob_str_4");
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table2->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_FIND,&ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table4->data,0,0);


    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table3->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_FIND,&ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table4->data,0,0);

    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\n"
                   "TEST8\n"
                   "volanie strfind nad lokalnumi premennymi\n");


    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str1", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str2", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "str3", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 4;
    stable_add_var(sym_tab, "str4", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 5;
    stable_add_var(sym_tab, "ret", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "qwertyuiopasdfghjkl");
    stable_add_var(sym_tab, "lit1", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "yuiopasdf");
    stable_add_var(sym_tab, "lit2", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "");
    stable_add_var(sym_tab, "lit3", tmp_var);
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "834uplkjrewqfds");
    stable_add_var(sym_tab, "lit4", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "lit1");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str2");
    ptr_to_table2 = stable_get_var(sym_tab, "lit2");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str3");
    ptr_to_table2 = stable_get_var(sym_tab, "lit3");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str4");
    ptr_to_table2 = stable_get_var(sym_tab, "lit4");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "str2");
    ptr_to_table3 = stable_get_var(sym_tab, "str3");
    ptr_to_table4 = stable_get_var(sym_tab, "ret");

    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_FIND, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table4->data, 0, 0);

    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_FIND, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table4->data, 0, 0);

    ptr_to_table3 = stable_get_var(sym_tab, "str4");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_FIND, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table4->data, 0, 0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST9\n"
                   "volanie substr nad globanymi premennymi\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"qwertyuiopasdfghjkl");
    ptr_to_table1 = stable_add_var(sym_tab,"str",tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    ptr_to_table4 = stable_add_var(sym_tab,"ret_str",tmp_var);

    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 4;
    ptr_to_table2 = stable_add_var(sym_tab, "i", tmp_var);
    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 6;
    ptr_to_table3 = stable_add_var(sym_tab, "n", tmp_var);

    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_SUBSTR, &ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table4->data,0,0);



    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);



    printf("\n\n"
                   "TEST10\n"
                   "volanie substr nad lokalnymou premennou\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str_ret", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 3;
    stable_add_var(sym_tab, "i", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 4;
    stable_add_var(sym_tab, "n", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "qwertyuiopasdfghjkl");
    stable_add_var(sym_tab, "lit1", tmp_var);

    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 4;
    stable_add_var(sym_tab,"4",tmp_var);

    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 6;
    stable_add_var(sym_tab,"6",tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str");
    ptr_to_table2 = stable_get_var(sym_tab, "lit1");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "i");
    ptr_to_table2 = stable_get_var(sym_tab, "4");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "n");
    ptr_to_table2 = stable_get_var(sym_tab, "6");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab,"str");
    ptr_to_table2 = stable_get_var(sym_tab,"str_ret");
    ptr_to_table3 = stable_get_var(sym_tab,"i");
    ptr_to_table4 = stable_get_var(sym_tab,"n");

    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_CALL_SUBSTR, &ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table1->data,0,0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);

    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\n"
                   "TEST11\n"
                   "volanie str_sort nad globanymi premennymi\n");

    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"qwert1234567890p-ojhgfdszx cvbnmklp[';,mnbvl64789324[p';/.yuiopasdfghjkl");
    str_append_chars(&tmp_var.data.data.s, str1);
    stable_add_var(sym_tab, "glob_str_1", tmp_var);

    tmp_var.data.data.s = str_init();
    tmp_var.data.arg_type = STRING;
    stable_add_var(sym_tab, "ret", tmp_var);

    ptr_to_table1 = stable_get_var(sym_tab,"glob_str_1");
    ptr_to_table4 = stable_get_var(sym_tab,"ret");

    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_SORT,&ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table4->data,0,0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST12\n"
                   "volanie str_sort nad lokalnumi premennymi\n");


    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str_ret", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "qwertyuiopasdf8348y5u438h;l3;t43ecfc/w"
            "['llkjghdslkhlkdhoi79843ytpoireuhkf j;noiervtw3o98trp98bewvt79438v7btp9ghjkl");
    stable_add_var(sym_tab, "lit1", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str");
    ptr_to_table2 = stable_get_var(sym_tab, "lit1");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);


    ptr_to_table1 = stable_get_var(sym_tab,"str");
    ptr_to_table2 = stable_get_var(sym_tab,"str_ret");

    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_SORT,&ptr_to_table2->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table2->data,0,0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST13\n"
                   "volanie str_len nad globalnymi premennymi\n");

    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s,"qwert1234567890p-ojhgfdszx cvbnmklp[';,mnbvl64789324[p';/.yuiopasdfghjkl");
    str_append_chars(&tmp_var.data.data.s, str1);
    stable_add_var(sym_tab, "glob_str_1", tmp_var);

    tmp_var.data.arg_type = INTEGER;
    stable_add_var(sym_tab, "ret", tmp_var);

    ptr_to_table1 = stable_get_var(sym_tab,"glob_str_1");
    ptr_to_table4 = stable_get_var(sym_tab,"ret");

    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_LEN,&ptr_to_table4->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table4->data,0,0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST14\n"
                   "volanie str_len nad lokalnymi premennymi\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str_ret", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "qwertyuiopasdf8348y5u438h;l3;t43ecfc/w"
            "['llkjghdslkhlkdhoi79843ytpoireuhkf j;noiervtw3o98trp98bewvt79438v7btp9ghjkl");
    stable_add_var(sym_tab, "lit1", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str");
    ptr_to_table2 = stable_get_var(sym_tab, "lit1");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);


    ptr_to_table1 = stable_get_var(sym_tab,"str");
    ptr_to_table2 = stable_get_var(sym_tab,"str_ret");
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_table1->data,0,0);
    create_and_add_instruction(i_list,INST_CALL_LEN,&ptr_to_table2->data,0,0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list,INST_WRITE,&ptr_to_table2->data,0,0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST15\n"
                   "volanie INST_EXPR_ADD_STR nad lokalnymi premennymi\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 1;
    stable_add_var(sym_tab, "str1", tmp_var);
    tmp_var.data.arg_type = STACK_EBP;
    tmp_var.data.data.i = 2;
    stable_add_var(sym_tab, "str2", tmp_var);
    tmp_var.data.arg_type = ON_TOP;
    tmp_var.data.data.i = 0;
    ptr_to_table3 = stable_add_var(sym_tab, "top", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "qwertyuiopasdf8348y5u438h;l3;t43ecfc/w"
            "['llkjghdslkhlkdhoi79843ytpoireuhkf j;noiervtw3o98trp98bewvt79438v7btp9ghjkl");
    stable_add_var(sym_tab, "lit1", tmp_var);
    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "q321321654984965132165498469513216519846413203218563516");
    stable_add_var(sym_tab, "lit2", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH_STRING, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "lit1");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str2");
    ptr_to_table2 = stable_get_var(sym_tab, "lit2");
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);

    ptr_to_table1 = stable_get_var(sym_tab, "str1");
    ptr_to_table2 = stable_get_var(sym_tab, "str2");
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);

    printf("\n\n"
                   "TEST16\n"
                   "volanie INST_EXPR_ADD_STR nad globalnymi premennymi\n"
                   "nachadzju sa tu aj datove typy double a int\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = ON_TOP;
    tmp_var.data.data.i = 0;
    ptr_to_table3 = stable_add_var(sym_tab, "top", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, "toto je string a za nim int : ");
    ptr_to_table1 = stable_add_var(sym_tab, "lit1", tmp_var);
    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, " !! toto je string a za nim double : ");
    ptr_to_table2 = stable_add_var(sym_tab, "lit2", tmp_var);

    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 5678;
    ptr_to_table3 = stable_add_var(sym_tab, "int", tmp_var);
    tmp_var.data.arg_type = DOUBLE;
    tmp_var.data.data.d = 36.436456;
    ptr_to_table4 = stable_add_var(sym_tab, "double", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);


    ptr_to_table3 = stable_get_var(sym_tab,"top");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);


    printf("\n\n"
                   "TEST17\n"
                   "volanie INST_EXPR_ADD_STR nad globalnymi premennymi\n"
                   "nachadzju sa tu aj datove typy double a int\n");
    i_list = init_inst_list();
    sym_tab = stable_init(10);

    tmp_var.data.arg_type = ON_TOP;
    tmp_var.data.data.i = 0;
    ptr_to_table3 = stable_add_var(sym_tab, "top", tmp_var);

    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, " toto je int; ");
    ptr_to_table1 = stable_add_var(sym_tab, "lit1", tmp_var);
    tmp_var.data.arg_type = STRING;
    tmp_var.data.data.s = str_init();
    str_append_chars(&tmp_var.data.data.s, " : a toto je double ;");
    ptr_to_table2 = stable_add_var(sym_tab, "lit2", tmp_var);

    tmp_var.data.arg_type = INTEGER;
    tmp_var.data.data.i = 5678;
    ptr_to_table3 = stable_add_var(sym_tab, "int", tmp_var);
    tmp_var.data.arg_type = DOUBLE;
    tmp_var.data.data.d = 36.436456;
    ptr_to_table4 = stable_add_var(sym_tab, "double", tmp_var);


    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table3->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table4->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table2->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_STR_ADD, 0, 0, 0);


    ptr_to_table3 = stable_get_var(sym_tab,"top");
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table3->data, 0, 0);


    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);
    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    stable_destroy(&sym_tab);



    return 0;
}