#include "scanner.h"
#include "ial.h"

void parser_clean()
{
}

char* debug_keyword(int type)
{
	static char* keywords[] = {"boolean","break","class","continue","do","double",
	"else","false","for","if","int","return","String","static","true","void","while",NULL};
	return keywords[type];
}

stab_t* staticSym = NULL;

void printToken()
{
	int typeOfToken = getLastToken();
	switch(typeOfToken)
	{
		case TOK_ID:
			printf("ID %s\n",getTokString());
			break;
		case TOK_SPECIAL_ID:
			printf("SPECIALID %s\n",getTokString());
			break;
		case TOK_KEYWORD:
			printf("KEYWORD %s\n", debug_keyword(getTokInt()));
			break;
		case TOK_CONST:
			printf("NUM %d\n",getTokInt());
			break;
		case TOK_DOUBLECONST:
			printf("DOUBLE %f\n",getTokDouble());
			break;
		case TOK_LITERAL:
			printf("LITERAL '%s' \n",getTokString());
			break;
		case TOK_RIGHT_BRACE:
			printf("}\n");
			break;
		case TOK_LEFT_BRACE:
			printf("{\n");
			break;
		case TOK_RIGHT_PAR:
			printf(")\n");
			break;
		case TOK_LEFT_PAR:
			printf("(\n");
			break;
		case TOK_LE:
			printf("<=\n");
			break;
		case TOK_GE:
			printf(">=\n");
			break;
		case TOK_EOF:
			printf("EOF\n");
			break;
		case TOK_DELIM:		// ;
			printf(";\n");
			break;
		case TOK_LIST_DELIM:	// ,
			printf(",\n");
			break;
		case TOK_ASSIGN:	// =
			printf("=\n");
			break;
		case TOK_EQ:		// ==
			printf("==\n");
			break;
		case TOK_NOTEQ:		// !=
			printf("!=\n");
			break;
		case TOK_LESS:		// <
			printf("<\n");
			break;
		case TOK_GREATER:	// >
			printf(">\n");
			break;
		case TOK_MUL:		// *
			printf("*\n");
			break;
		case TOK_DIV:		// 
			printf("/\n");
			break;
		case TOK_PLUS:		// +
			printf("+\n");
			break;
		case TOK_MINUS:		// >
			printf("-\n");
			break;
		default:
			printf("Unk token with type %d\n",getLastToken());
			break;
	}
}

int main(int argc, char* argv[])
{
	char name[256] = "source.java";
	if(argc >= 2)
		snprintf(name,255,"%s",argv[1]);

	int result = scanner_openFile(name);
	if(result)
	{
		for(int i = 0; i < 2; i++)
		{
			scanner_rewind();
			int typeOfToken;
			while((typeOfToken= getToken()) != TOK_ERROR)
			{
				static int count = 0;
				count++;
				printf("%3d. ",count);
				printToken(typeOfToken);
			} 
			if(getLastToken() != TOK_EOF)
			{
				printf("Lexical error has occured.\n");
				return 1;
			}
		}
		scanner_closeFile();
		
	} else {
		printf("Error has occured while opening the file %s.\n",name);	
	}
	return 0;
}
