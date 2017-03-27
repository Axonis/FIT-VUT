/*********************************************
 *   Program :   stack.c
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
 *   Notes   :   Implementation of stack
 ********************************************/
#include "stack.h"
#include "instruction_list.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>


stack_t *stack_init() {
    stack_t *stack;

    if ((stack = malloc(sizeof(stack_t) + sizeof(argument_var_t)*MINIMAL_MALLOC_SIZE)) == NULL){
        return NULL;
    }


    stack->size = MINIMAL_MALLOC_SIZE;
    stack->used = 0;
    stack->base = 0;

    return stack;
}

void stack_destroy(stack_t **stack){
    //string killing
    for (int i = 0; i < (*stack)->used; i++) {
        if ((*stack)->data[i].arg_type == STRING) {
            str_destroy(&(*stack)->data[i].data.s);
        }
    }
    free(*stack);
    *stack = NULL;
}

int stack_push(stack_t **stack, argument_var_t var) {
    if (stack[0]->size <= stack[0]->used){
        resize(stack);
    }

    stack[0]->data[stack[0]->used] = var;

    stack[0]->used++;
    return 0;
}

argument_var_t stack_pop(stack_t *stack) {
    argument_var_t pop_var;
    if(stack->used > 0) {
        pop_var = stack->data[stack->used - 1];
        if (pop_var.arg_type == STRING) {
            str_destroy(&pop_var.data.s);
        }
        stack->used--;
    } else {
        error_and_die(RUNTIME_ERROR, "Empty stack pop");
    }
    return pop_var;
}


argument_var_t stack_top(stack_t *stack){
    return stack->data[stack->used - 1];    //-1 je tam preto ze nepouzivam premennu stack.top ale stack.used
}

argument_var_t *stack_top_ptr(stack_t *stack) {
    return &stack->data[stack->used - 1];
}

argument_var_t stack_ebp_relative(stack_t *stack, int position){
    return stack->data[stack->base + position - 1];
}

argument_var_t *stack_ebp_relative_ptr(stack_t *stack, int position){
    return &stack->data[stack->base + position - 1];
}


int stack_actualize_from_ebp(stack_t *stack, argument_var_t arg, int position){
    stack->data[stack->base + position - 1] = arg;
    return 0;
}

argument_var_t stack_from_top(stack_t *stack, int position){
    return stack->data[stack->used - 1 - position];
}

argument_var_t *stack_from_top_ptr(stack_t *stack, int position) {
    return &stack->data[stack->used - 1 - position];
}


void print_stack(stack_t *stack) {
    printf("\n--------------- STACK ------------------\n");
    for (int i = 0; i < stack->used ; i++) {
        if (i == stack->base){
            printf("----------------   <-- BASE\n");
        }
        switch (stack->data[i].arg_type) {
            case INTEGER:
                printf("%d : %d\n", i, stack->data[i].data.i);
                break;
            case DOUBLE:
                printf("%d : %g\n", i, stack->data[i].data.d);
                break;
            case STRING:
                printf("%d : %s \t %p\n", i, stack->data[i].data.s.str, stack->data[i].data.s.str);
                break;
            case INSTRUCTION:
                printf("%d : %p\n", i, stack->data[i].data.instruction);
                break;
            case INTEGER_UNINIT:
                printf("%d : \n", i);
                break;
            case DOUBLE_UNINIT:
                printf("%d : \n", i);
                break;
            case STRING_UNINIT:
                printf("%d : \n", i);
                break;
        }
    }

    printf("\n---------------- STACK END ---------------\n");
}

void resize(stack_t **stack) {
    *stack = realloc(*stack,sizeof(stack_t) + sizeof(argument_var_t)*(stack[0]->used + MINIMAL_MALLOC_SIZE));
    stack[0]->size += MINIMAL_MALLOC_SIZE;
}