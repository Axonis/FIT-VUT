/*********************************************
 *   Program :   scanner.h
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
 *   Notes   :   Definition of scanner
 ********************************************/

#ifndef SCANNER_ALT
#define SCANNER_ALT

#include <stdio.h>
#include "str.h"


// dynamic strings used for scanning long entities (identifier names, numbers)
extern string_t first, second, literal;

// Defines possible types of attributes, carried by a token
typedef union	{
	char*	string;		// for ID/keywords
	double	real;		// for constant
	int	integer;	// for constant
} t_tokenData;

// Structed, used to carry lexical unit together with its attribute (data)
typedef struct {
	unsigned int 	type;
	t_tokenData	data;	
	// line specifies token position in source code
	int		line;
	// character specifies token position in source code
	int		character;
} t_token;



extern t_token	g_lastToken;

/*---------------------------------------------------------------------------*/
//	Scanner enums	
/*---------------------------------------------------------------------------*/
// Attributes(subtypes) of TOK_KEYWORD
enum keywords {
	KW_BOOL,	// boolean
	KW_BREAK,	// break
	KW_CLASS,	// class
	KW_CONTINUE,	// continue
	KW_DO,		// do
	KW_DOUBLE,	// double
	KW_ELSE,	// else
	KW_FALSE,	// false
	KW_FOR,		// for
	KW_IF,		// if
	KW_INT,		// int
	KW_RETURN,	// return
	KW_STRING,	// String
	KW_STATIC,	// static
	KW_TRUE,	// true
	KW_VOID,	// void
	KW_WHILE,	// while
};

// Types of lexemes (token lexical classes)
enum	tokenTypes {
	// ==   !=    <    >   <=   >=    +    -    *    /   ID    (    )
	// start of expr tokens
	TOK_EQ,			// ==
	TOK_NOTEQ,		// !=
	TOK_LESS,		// <
	TOK_GREATER,		// >
	TOK_LE,			// <=
	TOK_GE,			// >=
	TOK_PLUS,		// +
	TOK_MINUS,		// -
	TOK_MUL,		// *
	TOK_DIV,		// /
	TOK_ID,			// identifier
	TOK_LEFT_PAR,		// (
	TOK_RIGHT_PAR,		// )
	// end of expr tokens
	TOK_SPECIAL_ID,		// full identifier
	TOK_KEYWORD,		// reserved keyword
	//TOK_OPERATOR,		// operator (.+- etc) //TODO remove this kind of token
	TOK_LITERAL,		// text constant
	TOK_CONST,		// integer constant
	TOK_DOUBLECONST, 	// real constant
	TOK_DELIM,		// ;
	TOK_LIST_DELIM,		// ,
	TOK_LEFT_BRACE,		// {
	TOK_RIGHT_BRACE,	// } 
	TOK_ASSIGN,		// =
	TOK_EOF, 		// EOF
	TOK_ERROR		// pseudo-token to detect multiple EOFs
	};		


/*---------------------------------------------------------------------------*/
//	List of public functions provided by scanner 	
/*---------------------------------------------------------------------------*/

// Opens a source file 'filename'
int	scanner_openFile(char* fileName);

// Closes the source file
int	scanner_closeFile();

// Return tokens from the start of file
int	scanner_rewind();

// Process another token, update g_lastToken structure and return OK on succ 
int	getToken();
// Return token
int	ungetToken();

// Returns the type of last token
int	getLastToken();
//
// Returns integer value of last token
int	getTokInt();

// Returns double value of last token
double	getTokDouble();

// Returns string value of last token
char* 	getTokString();

// Returns last token line
int	getTokLine();
// Returns last token line
int	getTokTabs();
#endif
