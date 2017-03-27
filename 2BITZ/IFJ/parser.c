/*********************************************
 *   Program :   parser.c
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
 *   Notes   :   Implementation of parser
 ********************************************/


#include "parser.h"
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "interpret.h"
#include "ial.h"
#include "error.h"

#include "op-parser.h"


// global instance of Table of symbols
stab_t	*staticSym = NULL;
// instruction tape carrying user's program
instruction_list_t* insProgram	= NULL;
// instruction tape used to inicialize static vars
instruction_list_t* insInit	= NULL;

// global flag to detect the order of pass
int isSecondPass = 0;

// global name of class we are wrapped in
char*	parser_class = NULL;
// global name of function we are parsing in
char*	parser_fun = NULL;

// global counter used to compare lexical order of parsing symbol
unsigned int current_rank = 0;

// global counter to determine the count of local variables
// and is neccessary to set correct stack offset of local variables
int	localVariablesCount = 0;

/******************************************************************************
 			HEADERS
******************************************************************************/

int implicitConversion(int src, int dest);
int class_definition_list();
int type_specifier(int* type);
int iteration_statement();
int jump_statement();
int definition();
int block_items_list();
int compound_statement();
int function_parameters_list(data_t*);
int statement();
int parameter_definition();
int function_arguments_list(data_t**);
int builtin_print();
void generateStore(data_t* dest, data_t* src);

/******************************************************************************
 			SYNTACTIC UTILS
******************************************************************************/
int	isTokenKeyword(int desiredKeyword)
{
	return (getLastToken() == TOK_KEYWORD && getTokInt() == desiredKeyword);
}

int	isIdentifier()
{
	return (getLastToken() == TOK_ID
		||  getLastToken() == TOK_SPECIAL_ID);
}

int	isTokenTypeSpecifier()
{
	return (getLastToken() == TOK_KEYWORD && (
		getTokInt() == KW_INT ||
		getTokInt() == KW_DOUBLE||
		getTokInt() == KW_STRING));
}

// Static expressions are special cases of expressions as their code is
// generated to insInit instead of insProgram
int parse_static_expr(data_t* sym, bool generate)
{
	// update current symbol lexical order
	current_rank = sym->rank;
	instruction_list_t* tmp = NULL; 

	if(generate)
	{
		// swap instruction lists
		tmp = insProgram;
		insProgram = insInit;
		insInit = tmp;
	}
	int type = parse_expression(generate, false);

	if(generate)
		sym->is_inicialized = 1;
	
	if(generate)
	{
		if(implicitConversion(type, sym->type) == 0)
			error_and_die(SEMANTIC_TYPE_ERROR, "Invalid conversion");
		generateStore(sym, NULL);

		// swap instruction lists
		tmp = insProgram;
		insProgram = insInit;
		insInit = tmp;
	}
	return type;
}

/******************************************************************************
 			SEMANTIC UTILS
******************************************************************************/
// Hack to ensure detection of uninicialized static symbols
// -> change the type of all uninicialized symbols (required by intepreter)
void fixStaticVars()
{
	stab_element_t *current;
	if (staticSym!= NULL)
	{
		for (unsigned i = 0; i < staticSym->stab_size; i++) {
			current = staticSym->arr[i];
			while(current) {
				if(current->stab_content.is_inicialized != 1 &&
					current->stab_content.data.data.i == UNINITIALIZED)
				{
					int type = current->stab_content.data.arg_type;
					switch(type)
					{
						case INTEGER:
						case DOUBLE:
						case STRING:
							// SET it to uninit data
							//printf("Reseting %s\n", current->stab_key);
							current->stab_content.data.arg_type += 7;	
							break;
						default:
							break;
					}
				}		
			    current = current->stab_next;
		    }
		}
        }
}

// Verify if types are compatible
// Allowed conversions:
// type -> same type
// int -> double
// 0 = fail
int implicitConversion(int src, int dest)
{
	if(src == dest)
		return 1;
	// int -> double
	if(src == INTEGER && dest == DOUBLE)
		return 1;
	// the rest is incorrect
	return 0;
}

// Return ON_TOP symbol, used in interpret codes to access the top of stack
argument_var_t* getStackTop()
{
	// create the symbol just once and return its handle anytime afterwards
	static argument_var_t* top = NULL;
	if(top)
		return top;
	data_t stackTop;
	stackTop.data.arg_type = ON_TOP; 	
	stackTop.data.data.i = 0;
	stackTop.is_inicialized = 1;
	
	data_t* dttop = stable_add_variadic(staticSym,stackTop, 1,"ifj16.on_top");
	if(dttop)
		top = &dttop->data;
	return top;
}

// Generates a STORE operation (assign)
// assign SRC to DEST
// src = if src is NULL, the top of stack is taken into account and POP is generated
void generateStore(data_t* dest, data_t* src)
{
	argument_var_t* src_sym = (src != NULL) ? (&src->data) : (getStackTop());

	create_and_add_instruction(insProgram, INST_STORE, &dest->data, src_sym, 0);
	if(src_sym->arg_type == ON_TOP)
		create_and_add_instruction(insProgram, INST_POP,0,0,0);
}

// Generates a code which will call Main.run
void generateIntro()
{
	data_t* run = stable_search_variadic(staticSym, 1, "Main.run");
	
	argument_var_t* item = (run != NULL)?(&run->data):NULL;
	
	create_and_add_instruction(insProgram, INST_CALL, item,0,
		(argument_var_t*) 0x6);
	create_and_add_instruction(insProgram, INST_HALT, 0,0,0);
}

int isSymbolFunction(data_t* sym)
{
	if(sym != NULL)
		if(sym->data.arg_type == INSTRUCTION)
			return 1;
	return 0;
}

// Generates a sequence of code which will call either user function
// or built-in one, and clean the stack afterwards
void generateFunctionCall(data_t* func,data_t* retSym)
{
	// memory address to receive the return value
	argument_var_t* retVal = (retSym != NULL)?(&retSym->data):NULL;
	argument_var_t* callType = NULL; 
	
	// if VOID function is specified, then throw the return value away after return
	if(func->type == VOID)
		callType = (argument_var_t*) VOID;
	if(!func)
		return;

	// get the type of called function
	switch(func->data.data.i)
	{
		case BUILTIN_READ:
			switch(func->type)
			{
				case INTEGER:
					create_and_add_instruction(insProgram, INST_READ_INT, retVal,0,0);
					break;
				case DOUBLE:
					create_and_add_instruction(insProgram, INST_READ_DOUBLE, retVal,0,0);
					break;
				case STRING:
					create_and_add_instruction(insProgram, INST_READ_STRING, retVal,0,0);
					break;
			}
			break;
		case BUILTIN_LEN:
			create_and_add_instruction(insProgram, INST_CALL_LEN, retVal,0,0);
			break;
		case BUILTIN_SUBSTR:
			create_and_add_instruction(insProgram, INST_CALL_SUBSTR, retVal,0,0);
			break;
		case BUILTIN_CMP:
			create_and_add_instruction(insProgram, INST_CALL_CMP, retVal,0,0);
			break;
		case BUILTIN_FIND:
			create_and_add_instruction(insProgram, INST_CALL_FIND, retVal,0,0);
			break;
		case BUILTIN_SORT:
			create_and_add_instruction(insProgram, INST_CALL_SORT, retVal,0,0);
			break;
			
		default:
			create_and_add_instruction(insProgram, INST_CALL, &func->data,retVal,callType);
	}
	// now generates the stack clean up
	data_t* ptrParam = func->next_param;
	while(ptrParam)
	{
		// for each formal parameter in function definition, generate a pop
		create_and_add_instruction(insProgram, INST_POP, 0,0,0);
		ptrParam = ptrParam->next_param;
	}
}

// Sets up a default class symbol data
void fillClassData(data_t* data)
{
	data->is_inicialized = 1;
	data->type = INTEGER;
	data->data.arg_type = INTEGER;	
	data->data.data.i = PLACEHOLDER_CLASS;
}

// Sets up a default function symbol data
void fillFunctionData(data_t* data,int type)
{
	data->is_inicialized = 1;
	data->type = type;
	data->data.arg_type = INSTRUCTION;	
	data->data.data.instruction = NULL; 
	data->next_param = NULL;
}

// Sets up a default static  symbol data
void fillStaticVarData(data_t* data,int type)
{
	data->type = type;
	data->data.arg_type = type;	
	data->data.data.i = UNINITIALIZED;
	data->is_inicialized = 0;
	data->next_param = NULL;
}

// Initialize symbol's data if necessary
void inicializeData(data_t* data)
{
	if(data->type == STRING)
	{	
		data->data.data.s = str_init();
	}
}

// Sets up a default local variable with position 'stackPos' at the stack
void fillLocalVarData(data_t* data,int type, int stackPos)
{
	data->is_inicialized = 1;
	data->type = type;
	data->data.arg_type = STACK_EBP;	
	data->data.data.i = stackPos;
	data->next_param = NULL;
}

// This utility function creates an unique constat in Table of symbols
data_t* createConstant(int type, int iVal, double dVal, char* cVal)
{
	static int constNum = 0;
	char buffer[255];
	// create a new unique symbol name for constant
	snprintf(buffer,254,"%d",constNum++);

	data_t const_data;
	fillStaticVarData(&const_data,type);
	const_data.is_inicialized = 1;

	switch(type)
	{
		case INTEGER:
			const_data.data.data.i = iVal;
			break;
		case DOUBLE:
			const_data.data.data.d = dVal;
			break;
		case STRING:
			inicializeData(&const_data);
			str_append_chars(&const_data.data.data.s, cVal);
			break;
				
	}
	return stable_add_variadic(staticSym, const_data,2, "ifj16.const", buffer);
}

// Generates an unique label in table of symbols, append it into instruction tape 
// and returns a handle
data_t* generateLabel(instruction_list_t* list)
{
	static int labelCounter = 0;

	instruction_item_t* itemLabel = create_and_add_instruction(list, INST_LABEL, 0,0,0);

	data_t dataLbl;
	fillFunctionData(&dataLbl,VOID);
	dataLbl.data.data.instruction = itemLabel;	

	char buff[100];
	snprintf(buff,99,"%u", labelCounter++);
	return stable_add_variadic(staticSym,dataLbl, 3, "ifj16", "labels", buff);
}

// Fill the Table of symbols with signatures of builtin functions and with
// reserved class 'ifj16'
void addBuiltInToTable(stab_t* table)
{
	data_t data, *ptr;
	// class IFJ16
	fillClassData(&data);
	stable_add_var(table, "ifj16",data);
	// print 
	fillFunctionData(&data,VOID);
	data.data.data.i = BUILTIN_PRINT;
	stable_add_variadic(table,data,2,"ifj16","print");
	// readInt 
	fillFunctionData(&data,INTEGER);
	data.data.data.i = BUILTIN_READ;
	stable_add_variadic(table,data,2,"ifj16","readInt");
	// readDouble
	fillFunctionData(&data,DOUBLE);
	data.data.data.i = BUILTIN_READ;
	stable_add_variadic(table,data,2,"ifj16","readDouble");
	// readString
	fillFunctionData(&data,STRING);
	data.data.data.i = BUILTIN_READ;
	stable_add_variadic(table,data,2,"ifj16","readString");
	 
	// length 
	fillLocalVarData(&data,STRING, -2);
	ptr =  stable_add_variadic(table,data,3,"ifj16","length","s");

	fillFunctionData(&data,INTEGER);
	data.data.data.i = BUILTIN_LEN;
	data.next_param = ptr;
	stable_add_variadic(table,data,2,"ifj16","length");

	// substr
	fillLocalVarData(&data,INTEGER, -2);
	ptr =  stable_add_variadic(table,data,3,"ifj16","substr","n");
	fillLocalVarData(&data,INTEGER, -3);
	data.next_param = ptr;
	ptr =  stable_add_variadic(table,data,3,"ifj16","substr","i");
	fillLocalVarData(&data,STRING, -4);
	data.next_param = ptr;
	ptr =  stable_add_variadic(table,data,3,"ifj16","substr","s");
	fillFunctionData(&data,STRING);
	data.next_param = ptr;
	data.data.data.i = BUILTIN_SUBSTR;
	stable_add_variadic(table,data,2,"ifj16","substr");

	//compare 
	fillLocalVarData(&data,STRING, -2);
	ptr =  stable_add_variadic(table,data,3,"ifj16","compare","s2");
	fillLocalVarData(&data,STRING, -3);
	data.next_param = ptr;
	ptr =  stable_add_variadic(table,data,3,"ifj16","compare","s1");
	fillFunctionData(&data,INTEGER);
	data.next_param = ptr;
	data.data.data.i = BUILTIN_CMP;
	stable_add_variadic(table,data,2,"ifj16","compare");
	// find
	fillLocalVarData(&data,STRING, -2);
	ptr =  stable_add_variadic(table,data,3,"ifj16","find","s");
	fillLocalVarData(&data,STRING, -3);
	data.next_param = ptr;
	ptr =  stable_add_variadic(table,data,3,"ifj16","find","search");
	fillFunctionData(&data,INTEGER);
	data.next_param = ptr;
	data.data.data.i = BUILTIN_FIND;
	stable_add_variadic(table,data,2,"ifj16","find");

	//sort 
	fillLocalVarData(&data,STRING, -2);
	ptr =  stable_add_variadic(table,data,3,"ifj16","sort","s");
	fillFunctionData(&data,STRING);
	data.next_param = ptr;
	data.data.data.i = BUILTIN_SORT;
	stable_add_variadic(table,data,2,"ifj16","sort");



}
// A debug utility used to print data type
char* type2str(int type)
{
	static char* str[] = {"INTEGER","DOUBLE","STRING",NULL,NULL,NULL,"VOID"};
	return str[type];
}

// Set correct stack positions for function formal parameters
// Note: Pascal-like calling convention is utilized in our project. Thus, 
// in order to set correct position, one must wait untill all params are
// parsed and then set correct stack offsets to all parameters
void util_correctParamList(data_t* func)
{
	data_t* ptr = func->next_param;
	if(!ptr)
		return;
	int offset = 0;
	while(ptr)
	{
		if(ptr->next_param == NULL)
			offset = ptr->data.data.i + 2;	
		ptr = ptr->next_param;
	}
	
	ptr = func->next_param;
	while(ptr)
	{
		ptr->data.data.i -= offset;
		ptr = ptr->next_param;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Parsing functions 
///////////////////////////////////////////////////////////////////////////////

/*<expr>                         -> expression
<expr>                         -> eps
*/
/*
<type-specifier>               -> String
<type-specifier>               -> int
<type-specifier>               -> double
<type-specifier>               -> void
*/
int type_specifier(int* out_type)
{
	getToken();
	switch(getTokInt())
	{
		default:
			return SYN_ERR;
		case KW_INT:
			*out_type = INTEGER;
			break;
		case KW_DOUBLE:
			*out_type = DOUBLE;
			break;
		case KW_STRING:
			*out_type = STRING;
			break;
	}
	return SYN_OK;	
}


//<more-next>                    -> expression 
//<more-next>                    -> <identifier> ( <function-parameters-list> ) 
int more_next(data_t* var)
{

	getToken();
	int isIdent = isIdentifier();
	if(getToken() == TOK_LEFT_PAR && isIdent)
	{
		ungetToken();
		int isPrint = !strcmp(getTokString(),"ifj16.print");
		// is it a ID = ifj16.print() case ?
		if(isPrint)
		{
			getToken();
			builtin_print();
			// NOTE: it's a semantic error in any case, just check out the syntax
			error_and_die(RUNTIME_UNINITIALIZED,"Assigned void function to variable.");
		} else 
		{
			char* callName = getTokString();
			data_t*	func = NULL, *data = NULL;
			if(isSecondPass)
			{
				if(getLastToken() == TOK_ID)
					func = stable_search_variadic(staticSym, 2, parser_class, getTokString());
				else
					func = stable_search_variadic(staticSym, 1, getTokString());
				if(!func)
					error_and_die(SEMANTIC_ERROR,"Function '%s' not found.",getTokString());
				// TODO: verify ERROR code in this special case
				if(!isSymbolFunction(func))
					error_and_die(SEMANTIC_ERROR,"Expecting '%s' to be function", getTokString());
				if(implicitConversion(func->type, var->type) == 0)
					error_and_die(RUNTIME_UNINITIALIZED,"Function call type dismatch [%s]",callName);
				data = func->next_param;
			}
			getToken();

			if(function_arguments_list(&data) == SYN_ERR)
				return SYN_ERR;

			if(data != NULL)
				error_and_die(SEMANTIC_TYPE_ERROR,"Too few arguments in funciton call [%s]",callName);

			if(getToken() != TOK_RIGHT_PAR)
				error_and_die(SYNTAX_ERROR,"Expected )");
			if(isSecondPass)
			{
				generateFunctionCall(func,var);
			}
		}
		return SYN_OK;
	} else {
		ungetToken();
		ungetToken();
		int type = parse_expression(isSecondPass, false);

		//GEN("Verify if result of expr can be assigned. If do, generate assign");
		if(isSecondPass)
		{
			if(implicitConversion(type,var->type) == 0)
				error_and_die(SEMANTIC_TYPE_ERROR, "Invalid conversion");
			generateStore(var, NULL);
		}
	}	
	return SYN_OK;
}
// builtin_print is a special case of function call which supports
// a concatenation as an argument
int builtin_print()
{
	int paramCount = 0;
	if(getToken() == TOK_RIGHT_PAR)
		error_and_die(SEMANTIC_TYPE_ERROR,"Expected a term or concatenation");
	ungetToken();	
	do {
		data_t* var = NULL;
		int res = getToken();
		if(isSecondPass)
		{
			switch(res)
			{
				case TOK_ID:
					var = stable_search_variadic(staticSym, 3, parser_class,parser_fun,getTokString());
					if(!var)
						var = stable_search_variadic(staticSym, 2, parser_class,getTokString());
					if(!var)
						error_and_die(SEMANTIC_ERROR, "Undefined variable '%s'",getTokString());
					break;
				case TOK_SPECIAL_ID:
					var = stable_search_variadic(staticSym,1, getTokString());
					if(!var)
						error_and_die(SEMANTIC_ERROR, "Undefined variable '%s'",getTokString());
					break;	
				case TOK_CONST:
					var = createConstant(INTEGER, getTokInt(), 0,NULL);
					break;
				case TOK_DOUBLECONST:
					var = createConstant(DOUBLE, 0, getTokDouble(),NULL);
					break;
				case TOK_LITERAL:
					var = createConstant(STRING, 0, 0,getTokString());
					break;
				default:
					error_and_die(SYNTAX_ERROR,"Expected an identifier or a constant.");
			}
			
			if(!var)
				error_and_die(INTERNAL_ERROR,"Failed to create constant");
			if(var->data.arg_type == INSTRUCTION)
				error_and_die(SEMANTIC_ERROR, "Expected variable, got function symbol");

			// now generate PUSH
			create_and_add_instruction(insProgram, INST_PUSH, &var->data,0,0);
			paramCount++;
		} else {
			switch(res)
			{
				case TOK_ID:
				case TOK_SPECIAL_ID:
				case TOK_CONST:
				case TOK_DOUBLECONST:
				case TOK_LITERAL:
					break;
				default:
					error_and_die(SYNTAX_ERROR,"Expected an identifier or a constant.");
			}
		}
		
		res = getToken();
		if(res == TOK_RIGHT_PAR)
			break;
		if(res != TOK_PLUS)
			error_and_die(SYNTAX_ERROR,"Expected +");

	} while (1);
	if(isSecondPass)
	{
		data_t* varCount = createConstant(INTEGER, paramCount, 0,NULL);
		create_and_add_instruction(insProgram, INST_CALL_PRINT, &varCount->data,0,0);
		while(paramCount-- > 0)
			create_and_add_instruction(insProgram,INST_POP, 0,0,0);
	}
	if(getToken() != TOK_DELIM)
		error_and_die(SYNTAX_ERROR,"Expected ;");

	return SYN_OK;
	
}


//<next>                         -> = <more-next>
//<next>                         -> ( <function-parameters-list> ) 

int next(data_t* symbol,char* id)
{	
	int isPrint = !strcmp(getTokString(),"ifj16.print");
	switch(getToken())
	{
		case TOK_ASSIGN:
			if(more_next(symbol) == SYN_ERR)
				return SYN_ERR;
			if(getToken() != TOK_DELIM)
				error_and_die(SYNTAX_ERROR,"Expected ;");
			return SYN_OK;
		case TOK_LEFT_PAR:
			if(isPrint)
			{
				return builtin_print();
			} else {
				//GEN("Check if symbol '%s' is defined", getTokString());
			
				if(isSecondPass)
				{
					if(!isSymbolFunction(symbol))
						error_and_die(SEMANTIC_ERROR, "Expected a function symbol.");
				}
				
				data_t*	dt = NULL;
				if(isSecondPass)
					dt = symbol->next_param;
				if(function_arguments_list(&dt) == SYN_ERR)
					return SYN_ERR;	
				if(dt != NULL)
					error_and_die(SEMANTIC_TYPE_ERROR,"Too few arguments in funciton call [%s]",id);
				//GEN("Generate a function call INSTR");
				if(getToken() != TOK_RIGHT_PAR)
					error_and_die(SYNTAX_ERROR,"Expected )");
				if(getToken() != TOK_DELIM)
					error_and_die(SYNTAX_ERROR,"Expected ;");
				if(isSecondPass)
				{
					generateFunctionCall(symbol,NULL);
				}
				return SYN_OK;
			}
	}
	error_and_die(SYNTAX_ERROR,"Expected = or (");
	return SYN_ERR;
}


//<jump-statement>               -> return <expression> ;
int jump_statement()
{
	getToken();
	if(!isTokenKeyword(KW_RETURN))
		error_and_die(SYNTAX_ERROR,"Expected return");
	
	int hasExpression = 1;
	if(getToken() == TOK_DELIM)
		hasExpression = 0;
	ungetToken();

	data_t* fn = stable_search_variadic(staticSym, 2, parser_class, parser_fun);
	if(isSecondPass)
	{
		if(!fn)
			error_and_die(INTERNAL_ERROR,"Can't get the handle of function");
		if(fn->type == VOID && hasExpression)
		{
			error_and_die(SEMANTIC_TYPE_ERROR, "Void function can't return an expr");
		}
		if(fn->type != VOID && !hasExpression)
		{
			error_and_die(SEMANTIC_TYPE_ERROR, "Missing return value");
		} 
	}

	int type = 0;
	if(hasExpression)
	{
		type = parse_expression(isSecondPass, false);
	}
	if(isSecondPass)
	{
		if(fn->type == VOID)
		{
			create_and_add_instruction(insProgram,INST_RET, 0,0,0);
		} else {
			//TODO: use expression
			if(implicitConversion(type,fn->type) == 0)
				error_and_die(SEMANTIC_TYPE_ERROR, "Invalid conversion");
			create_and_add_instruction(insProgram,INST_RET, getStackTop() ,0,0);
		}
	}

	if(getToken() != TOK_DELIM)
		error_and_die(SYNTAX_ERROR,"Expected ; at the end of return statement.");

	//GEN("Generate RETURN instruction and expression");

	return SYN_OK;

}

//<iteration-statement>          -> while ( expression ) <statement>

int iteration_statement()
{
	instruction_item_t* skipJump = NULL; 
	data_t* lblTest = NULL;

	getToken();
	if(!isTokenKeyword(KW_WHILE))
		error_and_die(SYNTAX_ERROR,"Expected return");

	//GEN("Generate WHILE.test label"); 
	if(isSecondPass)
	{
		lblTest= generateLabel(insProgram);
	}
	if(getToken() != TOK_LEFT_PAR)
		error_and_die(SYNTAX_ERROR,"Expected (");

	parse_expression(isSecondPass, true);

	if(getToken() != TOK_RIGHT_PAR)
		error_and_die(SYNTAX_ERROR,"Expected )");

	if(isSecondPass)
	{
		skipJump = create_and_add_instruction(insProgram, INST_JZ,0,getStackTop(),0);
        create_and_add_instruction(insProgram, INST_POP,0,0,0);
		//GEN("Generate COMPARE and JUMP test"); 
	}
	if(statement() == SYN_ERR)
		return SYN_ERR;
	if(isSecondPass)
	{
		//GEN("Generate WHILE_SKIP label and altern CMP jmp addres");
		create_and_add_instruction(insProgram, INST_JMP, &lblTest->data,0,0);
		data_t* lblSkip = generateLabel(insProgram);
        create_and_add_instruction(insProgram, INST_POP,0,0,0);


		// now altern compare to jmp to skip
		skipJump->instruction.addr1 = &lblSkip->data;
	}
	return SYN_OK;
}

//<selection-statement>          -> if ( expression ) <statement>  __else <statement__ 
int selection_statement()
{
	instruction_item_t* selection = NULL;
	instruction_item_t* endJmp = NULL;
	data_t* lblElse = NULL;
	data_t* lblSkip = NULL;

	getToken();
	if(isTokenKeyword(KW_IF))
	{
		if(getToken() != TOK_LEFT_PAR)
			error_and_die(SYNTAX_ERROR,"Expected (");
		
		parse_expression(isSecondPass, true);

		if(isSecondPass)
		{
			//GEN("Generate a CMP and JUMP.");
			selection = create_and_add_instruction(insProgram, INST_JZ, 0,getStackTop(),0);
            create_and_add_instruction(insProgram, INST_POP,0,0,0);
        }
		if(getToken() != TOK_RIGHT_PAR)
			error_and_die(SYNTAX_ERROR,"Expected )");
	
		
		if(statement() == SYN_ERR)
			return SYN_ERR;		

		if(isSecondPass)
		{
			//GEN("Generate a JMP.");
			// jump to the end of IF-ELSE
			endJmp = create_and_add_instruction(insProgram, INST_JMP, 0,0,0);
			//GEN("Generate a IF_ELSE label and altern JMP");
			lblElse = generateLabel(insProgram);
			// altern the jmp after condition to jmp to ELSE branch
			selection->instruction.addr1 = &lblElse->data;
            create_and_add_instruction(insProgram, INST_POP,0,0,0);

        }

		getToken();
		if(!isTokenKeyword(KW_ELSE))
		{
			ungetToken();
			// its OK, no ELSE
			if(isSecondPass)
			{
				//GEN("Generate a IF_SKIP label and altern JMP.");
				lblSkip = generateLabel(insProgram);
				// altern JMP after first compound to jmp to the end
				endJmp->instruction.addr1 = &lblSkip->data;
			}
			return SYN_OK;
			//error_and_die(SYNTAX_ERROR,"Expected else");
		}

		if(statement() == SYN_ERR)
			return SYN_ERR;		
		
		if(isSecondPass)
		{
			//GEN("Generate a IF_SKIP label and altern JMP.");
			lblSkip = generateLabel(insProgram);
			// altern JMP after first compound to jmp to the end
			endJmp->instruction.addr1 = &lblSkip->data;
		}
		return SYN_OK;	
		
	}	
	error_and_die(SYNTAX_ERROR,"Expected IF");
	return SYN_ERR;
}

//<assign-statement>             -> <identifier> <next> ;
int assign_statement()
{
	getToken();
	if(isIdentifier())
	{	
		char* assign_id = getTokString();
		data_t* symbol = NULL;
		//GEN("Check if variable '%s'  is defined", getTokString());

		if(isSecondPass)
		{
			if(getLastToken() == TOK_ID)
			{
				symbol = stable_search_variadic(staticSym,3,parser_class,parser_fun,getTokString());
				if(!symbol)
					symbol = stable_search_variadic(staticSym,2,parser_class,getTokString());
			} else {
				symbol = stable_search_variadic(staticSym,1,getTokString());
			}
			if(!symbol)
				error_and_die(SEMANTIC_ERROR,"Symbol %s is missing",getTokString());
		}
		return next(symbol,assign_id);
	}
	error_and_die(SYNTAX_ERROR,"Expected identifier or full-identifier.");
	return SYN_ERR;
}
//<compound-statement>           -> { <block-items-list> }
int compound_statement()
{
	if(getToken() != TOK_LEFT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected {");

	if(block_items_list() == SYN_ERR)
		return SYN_ERR;

	if(getToken() != TOK_RIGHT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected {");
	return SYN_OK;
}

//<block-items-list>             -> <block-item> <block-items-list>
//<block-items-list>             -> eps
int block_items_list()
{
	int type = getToken();
	if(type == TOK_EOF)
		error_and_die(SYNTAX_ERROR,"Missing }");
	if(type == TOK_RIGHT_BRACE)
	{
		ungetToken();
		return SYN_OK;
	}
	ungetToken();
	
	if(statement() == SYN_ERR)	
		return SYN_ERR;

	return block_items_list();
}

//<statement>                    -> <jump-statement>
//<statement>                    -> <iteration-statement>
//<statement>                   -> <selection-statement>
//<statement>                   -> <assign-statement>
//<statement>                    -> <compound-statement>

int statement()
{
	int type = getToken();
	int val = getTokInt();
	ungetToken();
	switch(type)
	{
		case TOK_KEYWORD:
		{
			switch(val)
			{
				case KW_RETURN:
					return jump_statement();
				case KW_IF:
					return selection_statement();
				case KW_WHILE:
					return iteration_statement();
				default:
					error_and_die(SYNTAX_ERROR,"Got keyword, Expected if, while or return");
			}
		}
		case TOK_ID:
		case TOK_SPECIAL_ID:
			return assign_statement();	
		case TOK_LEFT_BRACE:
			return compound_statement();
		default:
			break;
	}
	error_and_die(SYNTAX_ERROR,"Expected a statement");
	return SYN_ERR;
}

//<argument-definition>	-> <term>
int argument_definition(data_t** fun)
{
	data_t*	var;
	// payload is used to inicialize constants
	if(isSecondPass)
	{
		if(!(*fun))
			error_and_die(SEMANTIC_TYPE_ERROR,"Too many arguments");
		switch(getToken())
		{
			case TOK_ID:
				var = stable_search_variadic(staticSym, 3, parser_class, parser_fun,getTokString());
				if(!var)
					var = stable_search_variadic(staticSym,2, parser_class,getTokString());
				if(!var)
					error_and_die(SEMANTIC_ERROR," '%s' is missing.", getTokString());
				if(var->data.arg_type == INSTRUCTION)
					error_and_die(SEMANTIC_ERROR, "Expected variable, got function symbol");
				break;
			case TOK_SPECIAL_ID:
				var = stable_search_variadic(staticSym, 2, parser_class ,getTokString());
				if(!var)
					error_and_die(SEMANTIC_ERROR,"'%s' is an undefined symbol.", getTokString());
				if(var->data.arg_type == INSTRUCTION)
					error_and_die(SEMANTIC_ERROR, "Expected variable, got function symbol");
				break;
			case TOK_LITERAL:
				var = createConstant(STRING, 0,0, getTokString());
				break;
			case TOK_CONST:
				var = createConstant(INTEGER, getTokInt(),0,0);
				break;
			case TOK_DOUBLECONST:
				var = createConstant(DOUBLE, 0,getTokDouble(),0);
				break;
			default:
				error_and_die(SYNTAX_ERROR,"Expected a term in function call");
		}
		if(!var)
			error_and_die(INTERNAL_ERROR, "Failed to create constant");

		// push variable
		if(isSecondPass)
			create_and_add_instruction(insProgram, INST_PUSH, &var->data,0,0);	

		if(implicitConversion(var->type,(*fun)->type) == 0)
			error_and_die(SEMANTIC_TYPE_ERROR,"SEM - argument type dismatch.");
		*fun = (*fun)->next_param;
	} else {
		switch(getToken())
		{
			case TOK_ID:
			case TOK_SPECIAL_ID:
			case TOK_LITERAL:
			case TOK_CONST:
			case TOK_DOUBLECONST:
				break;
			default:
				error_and_die(SYNTAX_ERROR,"Expected a term in function call");
		}
	}
	return SYN_OK;
}
//<more-function-arguments>     -> eps
//<more-function-arguments>     -> , <argument-definition> <more-function-arguments> 

int more_function_arguments(data_t** fun)
{
	switch(getToken())
	{
		case TOK_LIST_DELIM:
				if(argument_definition(fun) == SYN_ERR)
					return SYN_ERR;
				return more_function_arguments(fun);
		case TOK_RIGHT_PAR:
			ungetToken();
			return SYN_OK;
		default:
			error_and_die(SYNTAX_ERROR,"Expected ,");
	}
	return SYN_ERR;
			
}

//<function-arguments-list>     -> eps
//<function-arguments-list>     -> <argument-definition> <more-function-arguments>

int function_arguments_list(data_t** fun)
{
	if(getToken() == TOK_RIGHT_PAR)
	{
		ungetToken();
		return SYN_OK;
	} else {
		ungetToken();

		if(argument_definition(fun) == SYN_ERR)
			return SYN_ERR;
		return more_function_arguments(fun);
	}
}

data_t** last_param;
int	param_count;

//<more-function-parameters>     -> eps
//<more-function-parameters>     -> , <parameter-declaration> <more-function-parameters> 

int more_function_parameters()
{
	switch(getToken())
	{
		case TOK_LIST_DELIM:
				if(parameter_definition() == SYN_ERR)
					return SYN_ERR;
				return more_function_parameters();
		case TOK_RIGHT_PAR:
			ungetToken();
			return SYN_OK;
		default:
			error_and_die(SYNTAX_ERROR,"Expected ,");
	}
	return SYN_ERR;
			
}

//<function-parameters-list>     -> eps
//<function-parameters-list>     -> <parameter-definition> <more-function-parameters>

int function_parameters_list(data_t* sym)
{
	// init
	last_param = &sym->next_param;
	param_count = -1000;
	
	if(getToken() == TOK_RIGHT_PAR)
	{
		ungetToken();
		return SYN_OK;
	} else {
		ungetToken();

		if(parameter_definition() == SYN_ERR)
			return SYN_ERR;
		return more_function_parameters();
	}
}

//<parameter-definition>        -> <type-specifier> simple-identifier

int parameter_definition()
{
	int type;
	type_specifier(&type);
	if(type == VOID) 
		error_and_die(SYNTAX_ERROR,"Expected type-specifier (int,double,String)\n");

	if(getToken() != TOK_ID)
		error_and_die(SYNTAX_ERROR,"Expected a simple-identifier for formal parameter.");

	// in the first pass create all formal parameters = crucial
	if(!isSecondPass)
	{
		if(stable_search_variadic(staticSym, 3, parser_class, parser_fun, getTokString()))
			error_and_die(SEMANTIC_ERROR,"Formal parameter '%s' already declared.", getTokString());
		//GEN(">>> Add parameter '%s':'%s'",getTokString(),type2str(type));
		// create a new symbol	
		data_t data;
		fillLocalVarData(&data,type, ++param_count);
		data_t* dt = stable_add_variadic(staticSym, data, 3, parser_class,parser_fun,getTokString());
		// move to next param
		(*last_param) = dt;
		last_param = &dt->next_param;
	}
	
	if(isSecondPass)
	{
		data_t* fun = stable_search_variadic(staticSym, 2,parser_class,getTokString());
		if(isSymbolFunction(fun))
			error_and_die(SEMANTIC_ERROR,"Symbol '%s' already defined as function."
				"Trying to define a local var.", getTokString(), parser_class,parser_fun);	
		
	}

	return SYN_OK;
}

//<variable-initialization>        -> eps
//<variable-initialization>        -> = <more-next>
int variable_initialization(data_t* variable)
{
	int res = getToken();
	ungetToken();
	if(res == TOK_DELIM)
		return SYN_OK;
	if(getToken() != TOK_ASSIGN)
		error_and_die(SYNTAX_ERROR,"Expected = in inicialization");
	return more_next(variable);
}

//<local-definition>            -> <type-specifier> simple-identifier <variable-initialization> ;
int local_definition()
{
	data_t* def = NULL;
	int type;	
	if(type_specifier(&type) == SYN_ERR)
		error_and_die(SYNTAX_ERROR,"Expected type-specifier (int,double,String)\n");
	if(type == VOID)
		error_and_die(SYNTAX_ERROR,"Definition of void variable.");

	if(getToken() != TOK_ID)
		error_and_die(SYNTAX_ERROR,"Expected simple-id in local definition");
	char* var = getTokString();
	// generate locale vars in the second run
	if(isSecondPass)
	{
		if(stable_search_variadic(staticSym, 3,parser_class, parser_fun,var))
			error_and_die(SEMANTIC_ERROR,"Local symbol '%s' ['%s.%s'] redef."
			" already defined.", var, parser_class,parser_fun);	

		data_t* fun = stable_search_variadic(staticSym, 2,parser_class,var);
		if(isSymbolFunction(fun))
			error_and_die(SEMANTIC_ERROR,"Symbol '%s' already defined as function."
				"Trying to define a local var.", var, parser_class,parser_fun);	
		data_t data;
		fillLocalVarData(&data, type, ++localVariablesCount);
		def = stable_add_variadic(staticSym,data,3, parser_class,parser_fun,var);
	}
	
	if(isSecondPass)
	{
		switch(type)
		{
			case INTEGER:
				create_and_add_instruction(insProgram, INST_PUSH_INT,0,0,0);
				break;
			case STRING:
				create_and_add_instruction(insProgram, INST_PUSH_STRING,0,0,0);
				break;
			case DOUBLE:
				create_and_add_instruction(insProgram, INST_PUSH_DOUBLE,0,0,0);
				break;
		}
	}	
	if(variable_initialization(def) == SYN_ERR)
		return SYN_ERR;
	if(getToken() != TOK_DELIM)
		error_and_die(SYNTAX_ERROR,"Expected ; in local definition");

	
	//GEN("Local variable '%s' with type: %s",var,type2str(type));
	return SYN_OK;
}

//<function-body>               -> <statement> <function-body>
//<function-body>               -> <local-definition> <function-body>
//<function-body>               -> eps
int function_body()
{
	// on epsilon-case
	if(getToken() == TOK_RIGHT_BRACE)
	{
		ungetToken();
		return SYN_OK;
	}

	// declaration
	if(isTokenTypeSpecifier())
	{
		ungetToken();
		if(local_definition() == SYN_ERR)
			return SYN_ERR;
		return function_body();
	}
	ungetToken();	
	// statement
	if(statement() == SYN_ERR)
		return SYN_ERR;
	return function_body();
}



//<function-definition>         -> ( <function-parameters-list> ) { <function-body> }
int function_definition(data_t * sym)
{
	if(getToken() != TOK_LEFT_PAR)
		error_and_die(SYNTAX_ERROR,"Expected (");
	// semantic hack
	localVariablesCount = 0;
	if(!isSecondPass)
		fillFunctionData(sym,sym->type);

	if(function_parameters_list(sym) == SYN_ERR)
		return SYN_ERR;
	if(!isSecondPass)
	{
		util_correctParamList(sym);
	}
	if(getToken() != TOK_RIGHT_PAR)
		error_and_die(SYNTAX_ERROR,"Expected ')'");		
		
	if(getToken() != TOK_LEFT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected '{'");		
	if(isSecondPass)
	{
		//GEN("Generate and save function label in instruction list.");
		sym->data.data.instruction = create_and_add_instruction(insProgram, INST_LABEL,0,0,0);
	}
	if(function_body() == SYN_ERR)
		return SYN_ERR;
	if(getToken() != TOK_RIGHT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected '}'");		
	
	//GEN("Verify if RETURN was generated somewhere and clear LOCAL VARS");
	if(isSecondPass)
	{
		create_and_add_instruction(insProgram, INST_RET, 0,0,0);	
	}
	return SYN_OK;

}


//<more-definition>             -> <function-definition> 
//<more-definition>             -> ;
//<more-definition>             -> = <expr> ;

int more_definition(data_t* sym)
{
	switch(getToken())
	{
		case TOK_LEFT_PAR:
			ungetToken();
			return function_definition(sym);
			break;
		case TOK_ASSIGN:
			parser_fun = NULL;
			if(!isSecondPass)
			{
				inicializeData(sym);	
				if(sym->type == VOID)
					error_and_die(SYNTAX_ERROR,"Void variable definition");
			}
			
			//GEN("Assign value");

			// rank is used to detect the lexical order of static variables
			 parse_static_expr(sym,isSecondPass);

			
			if(getToken() != TOK_DELIM)
				error_and_die(SYNTAX_ERROR,"Missing ';' in definition");
			
			return SYN_OK;
		case TOK_DELIM:
			if(!isSecondPass)
			{
				inicializeData(sym);	
				if(sym->type == VOID)
					error_and_die(SYNTAX_ERROR,"Void variable definition");
			}
			return SYN_OK;
		default:
			error_and_die(SYNTAX_ERROR,"Expected ';','=' or '(' in static definition.");
	}
	return SYN_ERR;
	
}

// definition invariant
data_t*  util_parseIdentifier(int symbolType)
{
	if(getToken() != TOK_ID)
		error_and_die(SYNTAX_ERROR,"Expected simple-id");

	data_t* pData = NULL;
	if(!isSecondPass)
	{
		if(stable_search_variadic(staticSym, 2,parser_class,getTokString()))
			error_and_die(SEMANTIC_ERROR,"Static symbol %s.%s already declared",
				parser_class, getTokString());
		//GEN("Create a new static symbol '%s' and set its type to '%s'",getTokString(),type2str(symbolType));
		data_t data;
		fillStaticVarData(&data,symbolType);
		pData = stable_add_variadic(staticSym,data,2,parser_class, getTokString());
	} else {
		pData = stable_search_variadic(staticSym,2,parser_class,getTokString());		
	}
	parser_fun = getTokString();
	return pData;
}

//<definition>                  -> void simple-identifier <function-definition>

int definition_void()
{
	int type = VOID;
	data_t* staticSymbol = util_parseIdentifier(type);
	return function_definition(staticSymbol);
}


//<definition>                  -> static <type-specifier> simple-identifier <more-definition>

int definition()
{
	getToken();
	if(isTokenKeyword(KW_VOID))
		return definition_void();

	ungetToken();
	int type;
	if(type_specifier(&type) == SYN_ERR)
		error_and_die(SYNTAX_ERROR,"Expected type-specifier (int,double,String) or void\n");

	data_t* staticSymbol = util_parseIdentifier(type);
	return more_definition(staticSymbol);
}

//<class-body>                  -> epsilon
//<class-body>                  -> static <definition> <class-body> 
int class_body()
{
	if(getToken() == TOK_RIGHT_BRACE)
	{
		ungetToken();
		return SYN_OK;
	}
	if(!isTokenKeyword(KW_STATIC))
		error_and_die(SYNTAX_ERROR,"Expected keyword 'static' in class body.");
	if(definition() == SYN_ERR || class_body() == SYN_ERR)
		return SYN_ERR;

	return SYN_OK;
}


//<class-definition>            -> class <identifier> { <class-body> } 

int class_definition()
{
	getToken();
	if(!isTokenKeyword(KW_CLASS))
		error_and_die(SYNTAX_ERROR,"Expected keyword 'class'\n");	

	if(getToken() != TOK_ID)
		error_and_die(SYNTAX_ERROR,"Expected simple-identifier\n");

	// if it's the first pass
	if(!isSecondPass)
	{
		//GEN("Verify if class '%s' was not declared before. If not, add a new symbol into table", getTokString());
		if(stable_get_var(staticSym, getTokString()) != NULL)
		{
			// semantic error, class redefinition	
			error_and_die(SEMANTIC_ERROR,"Class redefinition");
		} else {
			// append new symbol 'class_name' to Table of Symbols
			data_t data;
			fillClassData(&data);
			stable_add_var(staticSym, getTokString(),data);
		}
	}
	parser_class = getTokString();

	if(getToken() != TOK_LEFT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected { in class definition\n");
	if(class_body() == SYN_ERR)
		return SYN_ERR;
	if(getToken() != TOK_RIGHT_BRACE)
		error_and_die(SYNTAX_ERROR,"Expected } in class definition\n");
	return SYN_OK;
}

//<class-definition-list>       -> <class-definition> <class-definition-list>
//<class-definition-list>       -> eps
int class_definition_list()
{
	int result = getToken();
	if(result == TOK_EOF || result == TOK_ERROR)
	{
		// apply epsilon
		ungetToken();
		return SYN_OK;
	}
	ungetToken();

	if(class_definition() == SYN_ERR)
		return SYN_ERR;
	return class_definition_list();
}



//<source-program>              -> <class-definition-list>
int source_program()
{
	int res = class_definition_list();
	// after first pass, turn into second pass
	isSecondPass = 1;
	return res;
}

void parser_init()
{
	// inicialize & fill default data
	insProgram = init_inst_list();
	insInit = init_inst_list();
	staticSym = stable_init(1024);
	addBuiltInToTable(staticSym);

}


void parser_clean()
{
#ifdef DEBUG
	if (staticSym)
		stable_print(staticSym);
	if (insProgram)
		inst_list_print(insProgram);
#endif

	if (insProgram)
		dest_inst_list(&insProgram);
	if (insInit)
		dest_inst_list(&insInit);
	if (staticSym)
		stable_destroy(&staticSym);
}

int main(int argc, char **argv)
{
	// if no INPUT file has been specified
	if (argc < 2) {
		error_and_die(INTERNAL_ERROR, "USAGE: filename");
	}
	if (scanner_openFile(argv[1])) {
		parser_init();
		// the first pass which gathers static symbols
		source_program();
		// generate instructions for calling main
		generateIntro();
		// process the code second time and this time generate code
		scanner_rewind();
		source_program();

		// check if Main.run is over there
		data_t *run = stable_search_variadic(staticSym, 1, "Main.run");
		if (!isSymbolFunction(run))
			error_and_die(SEMANTIC_ERROR, "Missing 'Main.run'");
		if (run->type != VOID || run->next_param != NULL)
			error_and_die(SEMANTIC_ERROR,
				      "Main.run must be void-type");

		// HACK: set all uninitialized static vars to correct type to 
		// ensure detection in interpret
		fixStaticVars();
#ifndef NOINTERPRET
		// run initialization of static symbols
		interpret(insInit, staticSym);
		// run the user program 
		interpret(insProgram, staticSym);
#endif
		error_and_die(SUCCESS_ERROR, "OK");
		return 0;
	}
	error_and_die(INTERNAL_ERROR, "Failed to open %s", argv[1]);
	return 1;
}
