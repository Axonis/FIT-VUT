/*********************************************
 *   Program :   error.c
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
 *   Notes   :   Implementation of global error module
 ********************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "error.h"
#include "ial.h"
#include "interpret.h"
#ifndef TEST
#include "scanner.h"
#include "parser.h"
#include "op-parser.h"
#endif
extern stab_t* staticSym; 


// Call module dtors
void clean_up()
{
#ifndef TEST
	// TODO: 
	// scanner clean up
	scanner_closeFile();
	// parser clean up
	parser_clean();
    //clean interpret
    interpret_clean();

	opparser_clean();
#endif
}


// Exit the process with 'errtype' return value and cleanup resources before leaving
void errorLeave(int errtype)
{
	clean_up();
	exit(errtype);
}

const char* getErrorName(enum errorTypes type)
{
	switch(type)
	{
		case SUCCESS_ERROR:
			return "DONE";
		case LEXICAL_ERROR:
			return "LEXICAL";
		case SYNTAX_ERROR:
			return "SYNTAX";
		case SEMANTIC_ERROR:
			return "SEMANTIC";
		case SEMANTIC_TYPE_ERROR:
			return "SEM.TYPE";
		case SEMANTIC_ERROR_REST:
			return "SEM.REST";
		case RUNTIME_READ_ERROR:
			return "RT_READ";
		case RUNTIME_UNINITIALIZED:
			return "RT_UINIT";
		case RUNTIME_NULLDIVISION:
			return "RT_NULL";
		case RUNTIME_ERROR:
			return "RUNTIME";
		case INTERNAL_ERROR:
			return "INTERNAL";
		default:
			break;
	}
	return "UNK";
}

void error_and_die(enum errorTypes type, const char *fmt, ...)
{
    if(type != SUCCESS_ERROR)
    {
	    va_list valist;
	    
	    va_start(valist, fmt);
	#ifndef TEST
	    fprintf(stderr,"[%s][%d:%d] ",getErrorName(type),getTokLine(),getTokTabs());
	#endif
	    vfprintf(stderr,fmt, valist);
	    fputc('\n',stderr);
	    va_end(valist);
    }
    errorLeave(type);
}

