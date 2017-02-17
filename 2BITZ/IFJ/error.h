/*********************************************
 *   Program :   error.h
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
 *   Notes   :   Definition of error module
 ********************************************/

// Have a look at page no.2 of assignment
enum errorTypes
{
	// it's ok
	SUCCESS_ERROR = 0,
	// Lex. error (bad float format)
	LEXICAL_ERROR = 1,
	// Syntax error (violation of syntax rules)
	SYNTAX_ERROR = 2,
	// Undefined class/function/var, reinitialization of class, etc.
	SEMANTIC_ERROR = 3,
	// Semantic incompatibility of types (function parameters, function values)
	SEMANTIC_TYPE_ERROR = 4,
	// The rest
	SEMANTIC_ERROR_REST = 6,
	// Error occured during IO operation with built-in func. readDouble(), etc.
	RUNTIME_READ_ERROR = 7,
	// Work with uninitialized variable
	RUNTIME_UNINITIALIZED = 8,
	// Null division
	RUNTIME_NULLDIVISION = 9,
	// The rest of run-times
	RUNTIME_ERROR = 10,
	// Internal error (malloc failure,etc)
	INTERNAL_ERROR = 99

};

// Call module dtors
void clean_up();


// Leave the process with error 'errtype'
void errorLeave(int errtype);

// Error and die with code
void error_and_die(enum errorTypes type, const char *fmt, ...);
