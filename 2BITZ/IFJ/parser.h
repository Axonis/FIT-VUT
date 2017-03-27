/*********************************************
 *   Program :   parser.h
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
 *   Notes   :   Definition of parser
 ********************************************/

//-----------------------------------------------------------------------------
//	Private variables
//-----------------------------------------------------------------------------
extern int isSecondPass;

// Return state
// NOTE: upon using a error.c, this might actually be obsolete
enum syntaxCorrectness {SYN_OK,SYN_ERR};

//#define throw(format,...) fprintf(stderr,"[%s:%d]: "format"\n", __FILE__,__LINE__,##__VA_ARGS__),SYN_ERR
#define throw(...) fprintf(stderr,"[Syntax error][line %d:%d]", getTokLine(),getTokTabs()),  fprintf(stderr,__VA_ARGS__),fprintf(stderr,"\n"),errorLeave(3),SYN_ERR

#define PLACEHOLDER_CLASS	1337
#define PLACEHOLDER_STATIC	123
#define PLACEHOLDER_LOCAL 	777

#define UNINITIALIZED		666


//-----------------------------------------------------------------------------
//	DEBUGING MESSAGES
//-----------------------------------------------------------------------------
#if DEBUG > 0
#define GEN(format,...) fprintf(stderr,"[Generate:%d:%d]: "format"\n", getTokLine(), getTokTabs(),##__VA_ARGS__)
#else
#define GEN(format,...)  
#endif

#if DEBUG > 1
#define hint(format,...) fprintf(stderr,"[Hint:%d:%d]: "format"\n", getTokLine(), getTokTabs(),##__VA_ARGS__)
#else
#define hint(format,...)  
#endif

//-----------------------------------------------------------------------------
//	PUBLIC FUNCTIONS
//-----------------------------------------------------------------------------

int source_program();

void parser_init();
void parser_clean();
