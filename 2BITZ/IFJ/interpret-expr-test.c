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

    input_data.data.arg_type = ON_TOP;
    input_data.data.data.i = 1;
    stable_add_var(sym_tab, stack_top, input_data);

    stable_print(sym_tab);

    //TEST1
    printf("#################### INTEGET-ONLY ##############\n");
    printf("####################TEST1#######################\n");
    printf("8 + 5 = 13 <<<>>> 8 5 +\nOcakavany vystup : 13\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_ADD, 0, 0, 0);

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
    printf("8 - 5 = 3 <<<>>> 8 5 -\nOcakavany vystup : 3\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_SUB, 0, 0, 0);

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
    printf("8 * 5 = 40 <<<>>> 8 5 *\nOcakavany vystup : 40\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_MUL, 0, 0, 0);

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
    printf("8 / 5 = 1 <<<>>> 8 5 /\nOcakavany vystup : 1\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_DIV, 0, 0, 0);

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
    printf("456 / 9 = 50 <<<>>> 456 9 /\nOcakavany vystup : 50\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_INT, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    ptr_to_table1->data.data.i = 456;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    ptr_to_table1->data.data.i = 9;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_DIV, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");

    printf("#################### DOUBLE-ONLY ##############\n");
    //TEST6
    printf("####################TEST6#######################\n");
    printf("123.456 + 56.333365 = 179,789365 <<<>>> 123.456 56.333365 +\nOcakavany vystup : 179,789365\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_DOUBLE, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 123.456;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 56.333365;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_ADD, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");


    //TEST7
    printf("####################TEST7#######################\n");
    printf("123.456 - 56.333365 = 67,122635 <<<>>> 123.456 56.333365 -\nOcakavany vystup : 67,122635\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_DOUBLE, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 123.456;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 56.333365;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_SUB, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST8
    printf("####################TEST8#######################\n");
    printf("123.456 * 56.333365 = 6954,69190944 <<<>>> 123.456 56.333365 *\nOcakavany vystup : 179,789365\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_DOUBLE, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 123.456;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 56.333365;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_MUL, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    //TEST9
    printf("####################TEST9#######################\n");
    printf("123.456 / 56.333365 = 2,191525395 <<<>>> 123.456 56.333365 /\nOcakavany vystup : 179,789365\n");
    i_list = init_inst_list();
    create_and_add_instruction(i_list, INST_PUSH_DOUBLE, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_a);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 123.456;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_b);
    ptr_to_table1->data.arg_type = DOUBLE;
    ptr_to_table1->data.data.d = 56.333365;
    create_and_add_instruction(i_list, INST_PUSH, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_EXPR_DIV, 0, 0, 0);
    ptr_to_table1 = stable_get_var(sym_tab, lokal_c);
    ptr_to_table2 = stable_get_var(sym_tab, stack_top);
    create_and_add_instruction(i_list, INST_STORE, &ptr_to_table1->data, &ptr_to_table2->data, 0);
    create_and_add_instruction(i_list, INST_POP, 0, 0, 0);
    create_and_add_instruction(i_list, INST_WRITE, &ptr_to_table1->data, 0, 0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, 0);

    interpret(i_list, sym_tab);
    dest_inst_list(&i_list);
    printf("\n\n");




    stable_destroy(&sym_tab);
    return 0;
}