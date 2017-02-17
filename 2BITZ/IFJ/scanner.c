/*********************************************
 *   Program :   scanner.c
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
 *   Notes   :   Implementation of scanner
 ********************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "str.h"
#include "error.h"

// One of ASCII characters to be ignored
#define CARRIAGE_RETURN 0xD

FILE*	fHandle = NULL;
string_t first, second, literal;

/* Single-linked list of tokens, used only by scanner module*/
typedef struct listTokenElement 
{
	t_token	token;
	struct listTokenElement* next;
	struct listTokenElement* prev;
} t_tokenElem;


// single linked-list used to store processed tokens
t_tokenElem* tokenList = NULL;

// pointer to current token
t_tokenElem*	currentToken = NULL;

// global structure to get the data returned by last getToken 
t_token	g_lastToken;

// the count of lines & chars
int lines = 0, chars = -1;


///////////////////////////////////////////////////////////////////////////////
// Utility func
///////////////////////////////////////////////////////////////////////////////

void freeToken(t_token* tok)
{
	// delete string
	if(tok->type == TOK_ID  
		|| tok->type == TOK_LITERAL
		|| tok->type == TOK_SPECIAL_ID)
	{
		free(tok->data.string);
	}
}

// Wrapped sgetc which enhances original getc to support file position
int sgetc(FILE* f)
{
	int result = fgetc(f);
	chars++;
	if(result == '\n')
	{
		lines++;
		chars = -1;
	}
	return result;	
}

// Wrapped sungetc which enhances original ungetc to support file position
int sungetc(int c, FILE* f)
{
	ungetc(c,f);
	chars--;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Utility func
///////////////////////////////////////////////////////////////////////////////

int	scanner_openFile(char* fileName)
{
	// try to open source file and store its handle to global-var
	fHandle = fopen(fileName,"r");	
	if(fHandle)
		return 1;
	return 0;
}

int	scanner_closeFile()
{
	// destroy local strings
	str_destroy(&first);
	str_destroy(&second);
	str_destroy(&literal);

	// if source file is still opened
	if(fHandle)
	{
		fclose(fHandle);
		fHandle = NULL;
	}	
	// dispose single linked list
	t_tokenElem *ptr = tokenList,*helpptr = tokenList;
	while(ptr)
	{
		helpptr = ptr;
		ptr = ptr->next;
		freeToken(&helpptr->token);
		free(helpptr);
	}
	tokenList = NULL;

	return 0;
}


int	scanner_rewind()
{
	// set pointer to the start of linked list
	currentToken = NULL;
	return 0;
}

// set token pointer to the previous one if possible or report error
int	ungetToken()
{
	if(currentToken)
		currentToken = currentToken->prev;
	return 0;
}

// Returns the type of last token
int	getLastToken()
{
	if(currentToken)
		return currentToken->token.type;
	else
		return TOK_ERROR;
}
//
// Returns integer value of last token
int	getTokInt()
{
	if(currentToken)
		return currentToken->token.data.integer;
	else
		return 0;
	
}

// Returns double value of last token
double	getTokDouble()
{
	if(currentToken)
		return currentToken->token.data.real;
	else
		return 0.0;
}
// Returns string value of last token
char* 	getTokString()
{
	if(currentToken)
		return currentToken->token.data.string;
	else
		return NULL;
}

// Returns last token line
int	getTokLine()
{
	if(currentToken)
		return currentToken->token.line;
	else
		return 0;
}
// Returns last token line
int	getTokTabs()
{
	if(currentToken)
		return currentToken->token.character;
	else
		return 0;
}

/* Utility */
char*	createString(const char* str)
{
	char* nstr = malloc(strlen(str)+1);
	if(nstr)
	{
		strcpy(nstr,str);
		return nstr;
	}
	return NULL;
}

// Utility function for fully-specified identifiers
char*	createString2(const char* str, const char* second)
{
	char* nstr = malloc(strlen(str)+strlen(second)+2);
	if(nstr)
	{
		sprintf(nstr,"%s.%s",str,second);
		return nstr;
	}
	return NULL;
}


// processes text literals such as "text" or "tex\t"
int	process_literal()
{
	str_reinit(&literal);
	sgetc(fHandle);
	// a normal state is used when awaiting regular ASCII input
	// SPECIAL state is reached after receiving '\' 
	enum States {NORMAL, SPECIAL,OCTAL};
	int c, state = NORMAL;

	int octBase= 64;
	char sum = 0;
	while((c = sgetc(fHandle)) != EOF)
	{
		switch(state)
		{
			case NORMAL:
				if(c <= 31)
					error_and_die(LEXICAL_ERROR, "Invalid character in text literal");
				if(c == '\"')
				{
					// when the end of terminal is reached
					g_lastToken.type = TOK_LITERAL;
					g_lastToken.data.string = createString(literal.str);
					// TODO: add pointer to symbol
					return TOK_LITERAL;

				} else if(c == '\\')
				{
					state = SPECIAL;
					break;
				} else {
					ADD_CHAR(literal,c);
				}
				break;
			// process escape sequences (e.g. \n) or report invalid ones
			case SPECIAL:
				switch(c)
				{
					case '0': case '1': case '2': case '3': 
						sungetc(c,fHandle);
						state = OCTAL;
						break;
					case '\\':
					case '\"':
                        			ADD_CHAR(literal,c);
						state = NORMAL;
						break;
					case 'n':
                        			ADD_CHAR(literal,'\n');
						state = NORMAL;
						break;
					case 't':
                        			ADD_CHAR(literal,'\t');
						state = NORMAL;
						break;
					default:
						fprintf(stderr, "LEXICAL Error while reading literal\n");
						errorLeave(LEXICAL_ERROR);
						return TOK_ERROR;
						break;
				}
				break;
			case OCTAL:
				if(isdigit(c))
				{
					// convert ASCII char to a number of <0,9>
					int digit = c-'0';
					if(digit > 7)
					{
						error_and_die(LEXICAL_ERROR, "Octal number out of range");
					}
					// and multiply it with N power of 8
					// => conversing octal-to-decal with one buffer sum 
					sum += octBase*digit;
					octBase /= 8;
					// if we have read 3 numbers
					if(octBase == 0)
					{
						if(sum == 0)
						{
							error_and_die(LEXICAL_ERROR, "Octal number out of range");
						}
						//concatenate a new char
						ADD_CHAR(literal,sum);
						octBase= 64;
						sum = 0;
						// and continue reading the rest of literal
						state = NORMAL;
					}
				} else {
					fprintf(stderr,"LEXICAL - Expected an octal digit\n");
					errorLeave(LEXICAL_ERROR);
					return TOK_ERROR;
				}	
				break;
		}
	}
	fprintf(stderr,"LEXICAL - Missing the end of literal\n");
	errorLeave(LEXICAL_ERROR);
	return TOK_ERROR;
}

// Utility function
// Returns 1 if ID is an keyword (type of keyword is returned in 2nd param)
// Returns 0 otherwise
int	isKeyword(const char* str,int* typeOfKeyword)
{
	static char* keywords[] = {"boolean","break","class","continue","do","double",
	"else","false","for","if","int","return","String","static","true","void","while",NULL};

	for(int i = 0; keywords[i] != NULL; i++)
	{
		if(strcmp(keywords[i],str) == 0)
		{
			(*typeOfKeyword) = i;
			return 1;
		}
	}
	return 0;
}

int	process_identifier()
{
	str_reinit(&first);
	str_reinit(&second);

	int c;

	enum states {FIRST,SECOND};
	int state = FIRST;
	while((c = sgetc(fHandle)) != EOF)
	{
		int res = isalnum(c);
	
		// if character fullfills ID requirements 
		if(res || c == '$' || c == '_')
		{
		
			// store the incoming char into either first or second part of ID
			if(state == FIRST) {
				str_add_char(&first,c);
			}else {
				str_add_char(&second, c);
			}
		} else {
			// if it is a ID splitter (FIRST.SECOND)
			if(c == '.')
			{
				// then change the state to second
				if(state == FIRST)
					state = SECOND;
				else {
					// already processing the second part of ID
					// > lex. error
					fprintf(stderr,"Error: Multiple '.' in identifier.\n");
					errorLeave(LEXICAL_ERROR);
					return TOK_ERROR;
				}
				continue;
			}
			
			// the last character is probably a part of another token->return back
			sungetc(c,fHandle);
			break;
		}
	}

	if (first.len) {
		// if we ended up in state FIRST, it can be a simple ID or keyword
		if (state == FIRST){
			int typeOfKeyword;
			// if it is a real keyword
			if(isKeyword(first.str,&typeOfKeyword))
			{
				g_lastToken.type = TOK_KEYWORD;
				g_lastToken.data.integer = typeOfKeyword;
				return TOK_KEYWORD;
			} else {
				g_lastToken.type = TOK_ID;
				g_lastToken.data.string = createString(first.str);
				return TOK_ID;
			}
		} else {
			int type;
			if (second.len){
				if(isdigit(second.str[0]))
					error_and_die(LEXICAL_ERROR, "The second part is invalid.");
				if(isKeyword(first.str,&type) || isKeyword(second.str,&type))
				{
					error_and_die(LEXICAL_ERROR, "Reserved word in fully qualified identifier");
				}
				g_lastToken.type = TOK_SPECIAL_ID;
				g_lastToken.data.string = createString2(first.str,second.str);
				return TOK_SPECIAL_ID;
			} else {
				error_and_die(LEXICAL_ERROR, "Second part of fully qualified ID is empty");
				return TOK_ERROR;
			}
		}
	}

	errorLeave(INTERNAL_ERROR);
	return TOK_ERROR;
	
}

int	process_number()
{
	str_reinit(&first);
	int c; 

	enum numberType {INT, DOT,DOUBLE,EXP,EXP_SIGN,EXP_RADIX};
	int state = INT;
	while((c = sgetc(fHandle)) != EOF)
	{
		switch(state)
		{
			case INT:
				if(isdigit(c) || tolower(c) == 'e' || c == '.')
				{
					ADD_CHAR(first,c);
					// on '.', move to REAL lexem
					if(c == '.')
						state = DOT;
					else if(tolower(c) == 'e')
						state = EXP;
				} else {
					// on anything else, it's an integer
					sungetc(c,fHandle);
					g_lastToken.type = TOK_CONST;
					g_lastToken.data.integer = atoi(first.str);
					return TOK_CONST;
				}
				break;
			case DOT:
				if(isdigit(c))
				{
					ADD_CHAR(first,c);
					state = DOUBLE;
				} else {
					// emit error, number ends with '.' without any following digit
					fprintf(stderr,"LEX: Expected a digit after . \n");
					errorLeave(LEXICAL_ERROR);	
					return TOK_ERROR;
				}
				break;
			case DOUBLE:
				if(!isdigit(c) && tolower(c) != 'e')
				{
					// new float
					sungetc(c,fHandle);
					g_lastToken.type = TOK_DOUBLECONST;
					g_lastToken.data.real= atof(first.str);
					return TOK_DOUBLECONST;
				} else {
					ADD_CHAR(first,c);
					if(tolower(c) == 'e')
						state = EXP;
				}
				break;
			case EXP:
				if(c == '+' || c == '-')
				{
					ADD_CHAR(first,c);
					state = EXP_SIGN;
				} else if(isdigit(c))
				{
					ADD_CHAR(first,c);
					state = EXP_RADIX;
				} else {
					fprintf(stderr,"LEX: Expected +- or digit after e \n");
					errorLeave(LEXICAL_ERROR);	
					return TOK_ERROR;
				}
				break;
			case EXP_SIGN:
				if(isdigit(c))
				{
					ADD_CHAR(first,c);
					state = EXP_RADIX;
				} else {
					fprintf(stderr,"LEX: Expected a digit after +- \n");
					errorLeave(LEXICAL_ERROR);	
					return TOK_ERROR;
				}
				break;
			case EXP_RADIX:
				if(isdigit(c) || c == '+' || c == '-')
				{
					ADD_CHAR(first,c);
				} else 
				{
					sungetc(c,fHandle);
					g_lastToken.type = TOK_DOUBLECONST;
					g_lastToken.data.real= atof(first.str);
					return TOK_DOUBLECONST;
				}
				break;
				
		}
	}

	// when  EOF has occured during the process
	
	if (first.len) {
		switch (state){
			case INT:
				g_lastToken.type = TOK_CONST;
				g_lastToken.data.integer = atoi(first.str);
				return TOK_CONST;
				break;
			case DOUBLE:
				g_lastToken.type = TOK_DOUBLECONST;
				g_lastToken.data.real= atof(first.str);
				return TOK_DOUBLECONST;
				break;

			case EXP_RADIX:
				g_lastToken.type = TOK_DOUBLECONST;
				g_lastToken.data.real= atof(first.str);
				return TOK_DOUBLECONST;
				break;
		}
	}
	errorLeave(LEXICAL_ERROR);
	return TOK_ERROR;
}

int	process_operator(char op)
{
	switch(op)
	{
		case '*':
			g_lastToken.type = TOK_MUL;		
			break;
		case '/':
			g_lastToken.type = TOK_DIV;		
			break;
		case '+':
			g_lastToken.type = TOK_PLUS;		
			break;
		case '-':
			g_lastToken.type = TOK_MINUS;		
			break;
		default:
			fprintf(stderr,"LEXICAL - Unknown operator '%c'\n",op);
			errorLeave(LEXICAL_ERROR);
			return TOK_ERROR;
	}
	return g_lastToken.type;
}

int	process_relation(char c)
{
	int nextc = sgetc(fHandle);
	if(nextc != EOF)
	{
		switch(c)
		{
			case '=':
				if(nextc == '=')
				{
					g_lastToken.type = TOK_EQ;
				} else {
					// assigment
					sungetc(nextc,fHandle);
					g_lastToken.type = TOK_ASSIGN;
				}
				return g_lastToken.type;
				break;
			case '!':
				if(nextc == '=')
					g_lastToken.type = TOK_NOTEQ;
				else {
					fprintf(stderr,"LEX: ! itself is not an operator, missing =");
					errorLeave(LEXICAL_ERROR);
					return TOK_ERROR;
				}
				return g_lastToken.type;
				break;
			case '<':
				if(nextc == '=')
					g_lastToken.type = TOK_LE;
				else {
					sungetc(nextc,fHandle);
					g_lastToken.type = TOK_LESS;
				}
				return g_lastToken.type;
				break;
			case '>':
				if(nextc == '=')
					g_lastToken.type = TOK_GE;
				else {
					sungetc(nextc,fHandle);
					g_lastToken.type = TOK_GREATER;
				}
				return g_lastToken.type;
				break;
			default:
				break;	
		}
	}
	errorLeave(LEXICAL_ERROR);
	return TOK_ERROR;
}
// Utility: maps ASCII symbols to token types
int	process_symbol(char op)
{
	int type = 0;
	switch(op)
	{
		case '(':
			type = TOK_LEFT_PAR;
			break; 
		case ')':
			type = TOK_RIGHT_PAR;
			break; 
		case '{':
			type = TOK_LEFT_BRACE;
			break; 
		case '}':
			type = TOK_RIGHT_BRACE;
			break; 
		case ';':
			type = TOK_DELIM;
			break; 
		case ',':
			type = TOK_LIST_DELIM;
			break;
		default:
			fprintf(stderr,"LEX: Unknown symbol \n'%c'",op);
			errorLeave(LEXICAL_ERROR);
			return TOK_ERROR;
	}		
	g_lastToken.type = type;
	return g_lastToken.type;
}

// parameter isBlock -> if func should process block comment
int	process_comments(int isBlock)
{
	// These states are used while waiting for a sequence to terminate
	// block commentary
	enum waitingState {FOR_END, NORMAL};
	enum commentaryType {LINE,BLOCK};
	int c,state = NORMAL;
	while((c = sgetc(fHandle)) != EOF)
	{
		switch(isBlock)
		{
			case LINE:
				// loop through text untill the end of line 
				if(c == '\n')
					return g_lastToken.type;
				break;
			case BLOCK:
				// either await a part of commentary or
				// wait for '/' in FOR_END after receiving '*'
				switch(state)
				{
					case NORMAL:
						if(c == '*')
							state = FOR_END;
						break;
					case FOR_END:
						if(c == '/')
							return g_lastToken.type;
						else if(c == '*')
							break;
						else
							state = NORMAL;
						break;
				}
		}
	}
	if(isBlock == LINE)
		return g_lastToken.type;

	fprintf(stderr,"LEXICAL - Missing */\n");
	errorLeave(LEXICAL_ERROR);
	return TOK_ERROR;	
}

// This function implements FSM as described in documentation. 
// In case the new token is processed correctly, g_lastToken structure is updated
int	intern_getToken()
{
	// let's get a character from source code's stream
	int c;
	while((c = sgetc(fHandle)) != EOF)
	{
		g_lastToken.line = lines;
		g_lastToken.character = chars;
		// if the stream is at its end, either return EOF token
		// or report error (multiple EOF tokens reached)
		if(c == EOF)
			break;
		// decide which type of lexem starts with 'c'
		switch(c)
		{
			// literals start with "
			case '\"':
				sungetc(c,fHandle);
				return  process_literal();
				break;
			// skip these chars
			case ' ':
			case '\t': 
			case '\n':
			case CARRIAGE_RETURN:
				break;
			// start of commentaries or '/' operator
			case '/':
				c = sgetc(fHandle);
				switch(c)
				{
					case EOF:
						break;
					// // or /* leads to commentary
					case '/':
					case '*':
						process_comments(c == '*');
						break;
					default:
						// it was an operator /
						sungetc(c,fHandle);
						return process_operator('/');
				}
				break;
			// math operators
			case '+':
			case '-':
			case '*':
				return process_operator(c);	
			// separators, delimeters, etc.
			case '{':
			case '}':
			case '(':
			case ')':
			case ';':
			case ',':
				return process_symbol(c);
			// <=, <, >, >=, ==, != 
			case '<':
			case '>':
			case '=':
			case '!':
				return process_relation(c);
			default:
				if(isalpha(c) || c == '_' || c == '$')
				{
					sungetc(c,fHandle);
					return process_identifier();
				} else if(isdigit(c))
				{
					sungetc(c,fHandle);
					return process_number();
				} else {
					error_and_die(LEXICAL_ERROR,"No token defined for 0x%X character\n",c);
					return TOK_ERROR;
				}
				break;
				
		}
	}
	// if there has already been an EOF token, report ERROR
	if(g_lastToken.type == TOK_EOF)
		return TOK_ERROR;

	// report the first occurence of EOF
	g_lastToken.type = TOK_EOF;
	return TOK_EOF;
}

// getToken returns another token from single-linked if any remained 
// or cause the processing of another one from source code
int getToken()
{
	// special case -> avoid TOK_EOF multiplication
	if(currentToken != NULL && currentToken->token.type == TOK_EOF)
		return TOK_ERROR;
	// special case for n+1th pass
	if(currentToken == NULL && tokenList != NULL)
	{
		currentToken = tokenList;
		g_lastToken = currentToken->token;
		return g_lastToken.type;
	}
	// if we don't have any token remaining in our linked list
	if(currentToken == NULL || currentToken->next == NULL)
	{
		// then process from file
		int ret = intern_getToken();	
		// get results and store them in double-linked-list
		t_tokenElem* newel = malloc(sizeof(t_tokenElem));
		if(newel)
		{
			newel->prev = currentToken;
			newel->next = NULL;
			newel->token = g_lastToken;
			if(currentToken)
				currentToken->next = newel;
			currentToken = newel;
			if(!tokenList)
				tokenList = newel;
			return ret;
		}
		// malloc error
		fprintf(stderr,"Internal eror - malloc failure \n");
		errorLeave(INTERNAL_ERROR);
		
		
	} 
	// otherwise move to the next in linked list
	currentToken = currentToken->next;	
	g_lastToken = currentToken->token;
	return g_lastToken.type;
}
