#include <stdio.h>
#include "ial.h"
#include "interpret.h"
#include "instruction_list.h"


int main () {
    stab_t *sym_tab;
    instruction_list_t *i_list;

    data_t input_data;
    data_t *ptr_to_table1, *ptr_to_table2;


    char lokal_a[] = "Main.Run.a";
    char lokal_b[] = "Main.Run.b";
    char lokal_c[] = "Main.Run.c";
    char lokald1[] = "lokal.d1";
    char lokald2[] = "lokal.d2";
    char stack_top[] = "stack-top";

    sym_tab = stable_init(1024);

    input_data.data.arg_type = INTEGER;
    input_data.data.data.i = 5;
    stable_add_var(sym_tab, lokal_b, input_data);

    input_data.data.arg_type = INTEGER;
    input_data.data.data.i = 8;
    stable_add_var(sym_tab, lokal_a, input_data);

    input_data.data.arg_type = STACK_EBP;
    input_data.data.data.i = 1;
    stable_add_var(sym_tab, lokal_c, input_data);

    input_data.data.arg_type = DOUBLE;
    input_data.data.data.d = 346.365;
    stable_add_var(sym_tab,lokald1,input_data);

    input_data.data.arg_type = DOUBLE;
    input_data.data.data.d = 45.6346;
    stable_add_var(sym_tab,lokald2,input_data);

    input_data.data.arg_type = ON_TOP;
    input_data.data.data.i = 1;
    stable_add_var(sym_tab, stack_top, input_data);

    stable_print(sym_tab);




    //TEST1
    printf("#################### INTEGET-ONLY ##############\n");
    printf("####################TEST1#######################\n");
    printf("8 < 5 = 13 <<<>>> 8 5 <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("####################TEST2#######################\n");
    printf("8 > 5  <<<>>> 8 5 >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("####################TEST3#######################\n");
    printf("8 <= 5 <<<>>> 8 5 <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("####################TEST4#######################\n");
    printf("8 >= 5  <<<>>> 8 5 >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST5
    printf("####################TEST5#######################\n");
    printf("8 == 5  <<<>>> 8 5 ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST5
    printf("####################TEST5.1#######################\n");
    printf("8 != 5  <<<>>> 8 5 !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST1
    printf("#################### Tests on same number ##############\n");
    printf("#################### TEST6 #######################\n");
    printf("5 < 5  <<<>>> 5 5 <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("#################### TEST7 #######################\n");
    printf("5 > 5  <<<>>> 5 5 >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("#################### TEST8 #######################\n");
    printf("5 <= 5 <<<>>> 5 5 <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("#################### TEST9 #######################\n");
    printf("5 >= 5  <<<>>> 5 5 >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST5
    printf("#################### TEST10 ######################\n");
    printf("5 == 5  <<<>>> 5 5 ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST5
    printf("####################TEST10.1#######################\n");
    printf("5 != 5  <<<>>> 5 5 !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST1
    printf("#################### DOUBLE-ONLY ##############\n");
    printf("#################### TEST11 #######################\n");
    printf("d1 < d2  <<<>>> d1 d2 <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("#################### TEST12 #######################\n");
    printf("d1 > d2  <<<>>> d1 d2 >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("#################### TEST13 #######################\n");
    printf("d1 <= d2 <<<>>> d1 d2 <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("#################### TEST14 #######################\n");
    printf("d1 >= d2  <<<>>> d1 d2 >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST5
    printf("#################### TEST15 #######################\n");
    printf("d1 == d2  <<<>>> d1 d2 ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST5
    printf("#################### TEST15.1 #######################\n");
    printf("d1 != d2  <<<>>> d1 d2 !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST1
    printf("#################### TEST16 #######################\n");
    printf("d1 < d1  <<<>>> d1 d1 <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("#################### TEST17 #######################\n");
    printf("d1 > d1  <<<>>> d1 d1 >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("#################### TEST18 #######################\n");
    printf("d1 <= d1 <<<>>> d1 d1 <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("#################### TEST19 #######################\n");
    printf("d1 >= d1  <<<>>> d1 d1 >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST5
    printf("#################### TEST20 #######################\n");
    printf("d1 == d1  <<<>>> d1 d1 ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST5
    printf("#################### TEST20.1 #######################\n");
    printf("d1 != d1  <<<>>> d1 d1 !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    stable_print(sym_tab);
    printf("#################### MIXED ##############\n");
    printf("#################### TEST21 #######################\n");
    printf("a < d2  <<<>>> a d2 <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("#################### TEST22 #######################\n");
    printf("a > d2  <<<>>> a d2 >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("#################### TEST23 #######################\n");
    printf("a <= d2 <<<>>> a d2 <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("#################### TEST24 #######################\n");
    printf("a >= d2  <<<>>> a d2 >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST5
    printf("#################### TEST25 #######################\n");
    printf("a == d2  <<<>>> a d2 ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    //TEST5
    printf("#################### TEST25.1 #######################\n");
    printf("a != d2  <<<>>> a d2 !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald2);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    stable_print(sym_tab);
    printf("#################### TEST26 #######################\n");
    printf("d1 < b  <<<>>> d1 b <\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOWER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST2
    printf("#################### TEST27 #######################\n");
    printf("d1 > b  <<<>>> d1 b >\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIGHER, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    //TEST3
    printf("#################### TEST28 #######################\n");
    printf("d1 <= b <<<>>> d1 b <=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_LOW_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST4
    printf("#################### TEST29 #######################\n");
    printf("d1 >= b  <<<>>> d1 b >=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_HIG_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);


    printf("#################### TEST25 #######################\n");
    printf("d1 == b  <<<>>> d1 b ==\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_EQUAL, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    printf("#################### TEST25.1 #######################\n");
    printf("d1 != b  <<<>>> d1 b !=\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokald1);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_NOT_EQ, 0, 0, 0);

    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");



    return 0;
}