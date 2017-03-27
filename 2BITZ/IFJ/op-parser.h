/*********************************************
 *   Program :   op-parser.h
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
 *   Notes   :   Definition of op parser
 ********************************************/

#ifndef _OP_PARSER_INCLUDED
#define _OP_PARSER_INCLUDED

#define MAX_TERMINALS   14
#define MAX_RULES       12

#include "ial.h"
#include "scanner.h"
#include "dynamic_stack.h"

enum 
{
    UNDEF  = -1,
    BOTTOM = 13,
    C      = 100
};

#define BOOL    1000

#ifdef OP_PARSER

// operator precedence table
const char op_table[][MAX_TERMINALS] =
{   //        0     1    2    3   4    5     6    7    8    9   10   11   12   13
    //        ==   !=    <    >   <=   >=    +    -    *    /   ID    (    )    $
    /* == */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* != */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* <  */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* >  */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* <= */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* >= */{'E', 'E', 'E', 'E', 'E', 'E', '<', '<', '<', '<', '<', '<', '>', '>' },
    /* +  */{'>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', '<', '>', '>' },
    /* -  */{'>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', '<', '>', '>' },
    /* *  */{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '>' },
    /* /  */{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '>' },
    /* ID */{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '_', '_', '>', '>' },
    /* (  */{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '_' },
    /* )  */{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '_', '_', '>', '>' },
    /* $  */{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '_', '=' },
};

// grammar for the expressions
const int rule_table[][MAX_RULES] =
{
    {C, C, TOK_EQ, C, UNDEF},
    {C, C, TOK_NOTEQ, C, UNDEF},
    {C, C, TOK_LESS, C, UNDEF},
    {C, C, TOK_GREATER, C, UNDEF},
    {C, C, TOK_LE, C, UNDEF},
    {C, C, TOK_GE, C, UNDEF},
    {C, C, TOK_PLUS, C, UNDEF},
    {C, C, TOK_MINUS, C, UNDEF},
    {C, C, TOK_MUL, C, UNDEF},
    {C, C, TOK_DIV, C, UNDEF},
    {C, TOK_ID, UNDEF, UNDEF, UNDEF},
    {C, TOK_LEFT_PAR, C, TOK_RIGHT_PAR, UNDEF}
};

// position of semantic information in rule symbols
const int rule_sempos[][2] =
{
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,2},
   {0,0},
   {1,1}
};
const int rule_len[MAX_RULES] =
{
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    2,
    4
};
#endif
int parse_expression(bool should_generate, bool is_condition);

int         find_right_side(const dstack_t *const);
dstack_t     get_reduce_symbols(const dstack_t const*, dstack_t *const);
const int*  get_rule(int);
void        dstack_reduce_rule(dstack_t *const, int);
expr_t get_top_terminal(dstack_t *);

void opparser_clean();



#endif
