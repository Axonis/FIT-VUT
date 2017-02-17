/*********************************************
 *   Program :   instruction_list.h
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
 *   Notes   :   Definition of instruction list
 ********************************************/


#ifndef INSTRUCTION_LIST
#define INSTRUCTION_LIST

#include "instructions.h"
#include "str.h"

//return values from functions
#define OK              0
#define MALLOC_ERROR    1

//variable type
#define ERROR           -1
#define INTEGER         0
#define DOUBLE          1
#define STRING          2
#define ON_TOP            3
#define STACK_EBP       4
#define INSTRUCTION     5
#define VOID            6
#define INTEGER_UNINIT  7
#define DOUBLE_UNINIT   8
#define STRING_UNINIT   9

enum buitInCodes { BUILTIN_INVALID, BUILTIN_PRINT, BUILTIN_CMP, BUILTIN_FIND, BUILTIN_SORT,
	 BUILTIN_SUBSTR, BUILTIN_LEN,BUILTIN_READ};
  
typedef union {
    int i;
    double d;
    string_t s;
    void *instruction;
} var_u;

typedef struct {
    int arg_type;
    var_u data;
} argument_var_t;

typedef struct {
    enum instructions type;
    argument_var_t *addr1;
    argument_var_t *addr2;
    argument_var_t *addr3;
} instruction_t;

typedef struct instruction_item  {
    instruction_t   instruction;
    struct instruction_item *next;
} instruction_item_t;

typedef struct {
    instruction_item_t *first;
    instruction_item_t *active;
    instruction_item_t *last;
} instruction_list_t;


instruction_list_t *init_inst_list();

int dest_inst_list(instruction_list_t **list);
int add_intstruction(instruction_list_t *i_list,instruction_item_t *instruction);

instruction_item_t *create_and_add_instruction(instruction_list_t *list, enum instructions type, argument_var_t *addr1,
                                               argument_var_t *addr2, argument_var_t *addr3);

void inst_list_print(instruction_list_t *list);


#endif //INSTRUCTION_LIST
