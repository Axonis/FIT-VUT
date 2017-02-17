#include <stdio.h>
#include "instruction_list.h"
#include "instructions.h"
#include "interpret.h"
#include "stable.h"

int main() {
    stab_t *sym_tab;
    instruction_list_t *i_list;
    instruction_item_t *ptr_inst_item;
    data_t tmp_data;
    data_t *ptr_to_tabl1,*p0,*p1,*p2,*on_stack,*el1,*el2,*a,*b,*n,*fib,*x,*main;

    sym_tab = stable_init(1024);
    i_list = init_inst_list();

    //hodnoty v symtab
    tmp_data.data.arg_type = STACK_EBP;
    tmp_data.data.data.i = -2;
    n = stable_add_var(sym_tab,"n",tmp_data);
    tmp_data.data.arg_type = INTEGER;
    tmp_data.data.data.i = 0;
    p0 = stable_add_var(sym_tab,"0",tmp_data);
    tmp_data.data.arg_type = INTEGER;
    tmp_data.data.data.i = 1;
    p1 = stable_add_var(sym_tab,"1",tmp_data);
    tmp_data.data.data.i = 2;
    p2 = stable_add_var(sym_tab,"2",tmp_data);
    tmp_data.data.arg_type = STACK_EBP;
    tmp_data.data.data.i = 1;
    a = stable_add_var(sym_tab,"a",tmp_data);
    x = stable_add_var(sym_tab,"x",tmp_data);
    tmp_data.data.arg_type = STACK_EBP;
    tmp_data.data.data.i = 2;
    b = stable_add_var(sym_tab,"b",tmp_data);
    tmp_data.data.arg_type = ON_TOP;
    tmp_data.data.data.i = 0;
    on_stack = stable_add_var(sym_tab,"on_stack",tmp_data);
    el1 = stable_add_var(sym_tab,"el1",tmp_data);
    el2 = stable_add_var(sym_tab,"el2",tmp_data);
    main = stable_add_var(sym_tab,"main",tmp_data);


/*
 int Fibonacci(int n)
{
   if ( n == 0 )
      return 0;
   else                 label el1
      if ( n == 1 )
        return 1;
      else              label el2
        int a = Fibonacci(n-1);
        int b =  Fibonacci(n-2);
        a = a+b;
        return a;
} //label end fib
 */

    //pred zacatim programu skoci do main
    create_and_add_instruction(i_list,INST_JMP,&main->data,0,0);

    //zaciatok int Fibonacci(int n)
    ptr_inst_item = create_and_add_instruction(i_list,INST_LABEL,0,0,0);
    tmp_data.data.data.instruction = ptr_inst_item;
    tmp_data.data.arg_type = INSTRUCTION;
    fib = stable_add_var(sym_tab,"Fibonacci",tmp_data);
    ptr_to_tabl1 = stable_get_var(sym_tab,"n");

    create_and_add_instruction(i_list,INST_PUSH,&p0->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_tabl1->data,0,0);
    create_and_add_instruction(i_list,INST_EXPR_EQUAL,0,0,0);
    create_and_add_instruction(i_list,INST_JZ,&el1->data,&on_stack->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);
    create_and_add_instruction(i_list,INST_RET,&p0->data,0,0);


    ptr_inst_item = create_and_add_instruction(i_list,INST_LABEL,0,0,0);
    el1->data.arg_type = INSTRUCTION;
    el1->data.data.instruction = ptr_inst_item;
    create_and_add_instruction(i_list,INST_POP,0,0,0);

    create_and_add_instruction(i_list,INST_PUSH,&p1->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&ptr_to_tabl1->data,0,0);
    create_and_add_instruction(i_list,INST_EXPR_EQUAL,0,0,0);
    create_and_add_instruction(i_list,INST_JZ,&el2->data,&on_stack->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);
    create_and_add_instruction(i_list,INST_RET,&p1->data,0,0);
    ptr_inst_item = create_and_add_instruction(i_list,INST_LABEL,0,0,0);
    el2->data.arg_type = INSTRUCTION;
    el2->data.data.instruction = ptr_inst_item;
    create_and_add_instruction(i_list,INST_POP,0,0,0);
    create_and_add_instruction(i_list,INST_PUSH_INT,0,0,0);
    create_and_add_instruction(i_list,INST_PUSH_INT,0,0,0);

    create_and_add_instruction(i_list,INST_PUSH,&n->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&p1->data,0,0);
    create_and_add_instruction(i_list,INST_EXPR_SUB,0,0,0);
    create_and_add_instruction(i_list,INST_CALL,&fib->data,&a->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);

    create_and_add_instruction(i_list,INST_PUSH,&n->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&p2->data,0,0);
    create_and_add_instruction(i_list,INST_EXPR_SUB,0,0,0);
    create_and_add_instruction(i_list,INST_CALL,&fib->data,&b->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);

    create_and_add_instruction(i_list,INST_PUSH,&a->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&b->data,0,0);
    create_and_add_instruction(i_list,INST_EXPR_ADD,0,0,0);
    create_and_add_instruction(i_list,INST_STORE,&a->data,&on_stack->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);
    create_and_add_instruction(i_list,INST_RET,&a->data,0,0);

    //MAIN beginning
/*
 * int main(){
 *      int x;
 *      x = readint();
 *      x = Fibonacci(x);
 *      print(x);
 *      exit(0);
 * }
 */
    ptr_inst_item = create_and_add_instruction(i_list,INST_LABEL,0,0,0);
    main->data.arg_type = INSTRUCTION;
    main->data.data.instruction = ptr_inst_item;
    create_and_add_instruction(i_list,INST_PUSH_INT,0,0,0);
    create_and_add_instruction(i_list,INST_READ_INT,&x->data,0,0);
    create_and_add_instruction(i_list,INST_PUSH,&x->data,0,0);
    create_and_add_instruction(i_list,INST_CALL,&fib->data,&x->data,0);
    create_and_add_instruction(i_list,INST_POP,0,0,0);
    create_and_add_instruction(i_list,INST_WRITE,&x->data,0,0);
    create_and_add_instruction(i_list, INST_HALT, 0, 0, (void *) 1);

    inst_list_print(i_list);
    interpret(i_list,sym_tab);

    stable_destroy(&sym_tab);
    dest_inst_list(&i_list);
    return 0;
}