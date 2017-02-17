/*********************************************
 *   Program :   instructions.h
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
 *   Notes   :   Defintion of instructions
 ********************************************/

#ifndef INSTRUCTIONS
#define INSTRUCTIONS

//TODO :  v klude dopisujte instrukcie ake potrebujete
enum instructions {     // ADDR1            ADDR2           ADDR3
    INST_NOP,           //no inputs
    INST_ADD,           // destination      operand1        operand2
    INST_SUB,           // destination      operand1        operand2
    INST_MUL,           // destination      operand1        operand2
    INST_DIV,           // destination      operand1        operand2
    INST_EXPR_ADD,      //take last 2 operands from stack
    INST_EXPR_SUB,      //take last 2 operands from stack
    INST_EXPR_MUL,      //take last 2 operands from stack
    INST_EXPR_DIV,      //take last 2 operands from stack
    INST_EXPR_LOWER,    //take last 2 operands from stack
    INST_EXPR_HIGHER,   //take last 2 operands from stack
    INST_EXPR_LOW_EQ,   //take last 2 operands from stack
    INST_EXPR_HIG_EQ,   //take last 2 operands from stack
    INST_EXPR_EQUAL,    //take last 2 operands from stack
    INST_EXPR_NOT_EQ,   //take last 2 operands from stack
    INST_EXPR_STR_ADD,  //take last 2 operands from stack
    INST_PUSH,          //var to push       ---             ---
    INST_PUSH_INT,      //---               ---             ---
    INST_PUSH_DOUBLE,   //---               ---             ---
    INST_PUSH_STRING,   //---               ---             ---
    INST_POP,           //no inputs
    INST_STORE,         //destination       source          ---
    INST_JMP,           //addr to jump      ---             ---
    INST_JEQ,           //ddr to jump       value           value
    INST_JNEQ,          //ddr to jump       value           value
    INST_JZ,            //addr to jump      value for equation  ---
    INST_JNZ,           //addr to jump      value for equation  ---
    INST_CALL,          //addr of func      where to save return value or nothing     ---
    INST_CALL_PRINT,    //num_of_strings    ---             ---
    INST_CALL_CMP,      //return_value      ---             ---
    INST_CALL_FIND,     //return value      ---             ---
    INST_CALL_SORT,     //return_value      ---             ---
    INST_CALL_SUBSTR,   //return_value      ---             ---
    INST_CALL_LEN,      //return_value
    INST_RET,           //value for return  ---             ---
    INST_WRITE,         //source            ---             ---
    INST_READ_INT,      //destination       ---             ---
    INST_READ_DOUBLE,   //destination       ---             ---
    INST_READ_STRING,   //destination       ---             ---
    INST_STR_INIT,      //str to init       ---             ---
    INST_STR_REINIT,    //str to reinit     ---             ---
    INST_STR_LEN,       //destination       source          ---
    INST_STR_CONCATENATE,//destination      str1            str2
    INST_STR_CMP,       //destination       str1            str2
    INST_HALT,          //no imputs
    INST_LABEL,         //no inputs
};

#endif //INSTRUCTIONS