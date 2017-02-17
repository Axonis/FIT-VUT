/*********************************************
 *   Program :   instruction_list.c
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
 *   Notes :     Implementation of instruction list
 ********************************************/

#include "instruction_list.h"

#include <stdlib.h>
#include <stdio.h>


instruction_list_t *init_inst_list() {
    instruction_list_t *new_list;
    instruction_item_t *new_instruction;

    if((new_list = malloc(sizeof(instruction_list_t))) == NULL) {
        return NULL;
    }

    //vytvorenie prazdnej instrukcie pre odstranenie podmienky v create and add instruction
    if((new_instruction = malloc(sizeof(instruction_item_t))) == NULL) {
        free(new_list);        //deleting list if second malloc fails
        return NULL;
    }

    new_instruction->instruction.addr1 = 0;
    new_instruction->instruction.addr2 = 0;
    new_instruction->instruction.addr3 = 0;
    new_instruction->instruction.type = INST_NOP;
    new_instruction->next = NULL;

    new_list->active = NULL;
    new_list->first  = new_instruction;
    new_list->last   = new_instruction;

    return new_list;
}


int dest_inst_list(instruction_list_t **list) {
    instruction_item_t *current_item, *next_item;

    if (*list != NULL) {
        current_item = (*list)->first;
        while (current_item != NULL) {          //cycle through all items in list
            next_item = current_item->next;
            free(current_item);                //deleting of each item of list
            current_item = next_item;
        }

        free(*list);                            //deleting of whole lsit
        *list = NULL;
    }
    return OK;
}


instruction_item_t *create_and_add_instruction(instruction_list_t *list, enum instructions type, argument_var_t *addr1,
                                               argument_var_t *addr2, argument_var_t *addr3) {

    instruction_item_t *new_instruction;

    if ((new_instruction = malloc(sizeof(instruction_item_t))) == NULL) {
        return NULL;
    }

    new_instruction->instruction.addr1 = addr1;
    new_instruction->instruction.addr2 = addr2;
    new_instruction->instruction.addr3 = addr3;
    new_instruction->instruction.type  = type;
    new_instruction->next              = NULL;

    list->last->next = new_instruction;
    list->last = new_instruction;

    return new_instruction;
}

void inst_list_print(instruction_list_t *list) {
    char *inst[] = {
            "INST_NOP",
            "INST_ADD",
            "INST_SUB",
            "INST_MUL",
            "INST_DIV",
            "INST_EXPR_ADD",
            "INST_EXPR_SUB",
            "INST_EXPR_MUL",
            "INST_EXPR_DIV",
            "INST_EXPR_LOWER",
            "INST_EXPR_HIGHER",
            "INST_EXPR_LOW_EQ",
            "INST_EXPR_HIG_EQ",
            "INST_EXPR_EQUAL",
            "INST_EXPR_NOT_EQ",
            "INST_EXPR_STR_ADD",
            "INST_PUSH",
            "INST_PUSH_INT",
            "INST_PUSH_DOUBLE",
            "INST_PUSH_STRING",
            "INST_POP",
            "INST_STORE",
            "INST_JMP",
            "INST_JEQ",
            "INST_JNEQ",
            "INST_JZ",
            "INST_JNZ",
            "INST_CALL",
            "INST_CALL_PRINT",
            "INST_CALL_CMP",
            "INST_CALL_FIND",
            "INST_CALL_SORT",
            "INST_CALL_SUBSTR",
            "INST_CALL_LEN",
            "INST_RET",
            "INST_WRITE",
            "INST_READ_INT",
            "INST_READ_DOUBLE",
            "INST_READ_STRING",
            "INST_STR_INIT",
            "INST_STR_REINIT",
            "INST_STR_LEN",
            "INST_STR_CONCATENATE",
            "INST_STR_CMP",
            "INST_HALT",
            "INST_LABEL",
            NULL,
    };
    int i = 0;
    for (instruction_item_t *current = list->first;
         current != NULL;
         current = current->next) {
        printf("%3d: %-20s %-10p %-10p %-10p\n", i++, inst[current->instruction.type],
               (void *) current->instruction.addr1, (void *) current->instruction.addr2,
               (void *) current->instruction.addr3);
    }
}
