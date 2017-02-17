# IFJ16 interpret

## About
IFJ16 is a four-to-five-members school project, assigned in __Formal languages and compilers [IFJ]__ subject. The goal is create
an interpreter for fictive language IFJ16, whose grammar can be gound in __doc/LL-grammar.txt__. 

The implementation has been split up into several modules:
* scanner = lexical analysis 
* parser = syntactic & semantic analysis & code generation
* op-parser = a sub-parser for parsing expressions
* interpreter = a module handling the execution of compiled program
* stable = table of symbols

## How it did at tests

Our project has fulfilled **97 percents** upon submit and received **20+4** points. For a complex analysis see __hodnoceni__ file. 

## How to compile

### How to run the parser
>`make`
>`./run PATH_TO_FILE`

### How to test the project
A bunch of regressive tests has been put together in order to detect any violations against previous work and assignment at the same time. Tests can be started using following command:

>`make test`

## Authors
	xbarna02 - Barna Kristian
	xdobia11 - Dobias Roman
	xsulca00 - Sulc Adam	
	xtomas32 - Tomasov Adrian
	xurban66 - Urbanovsky Jozef
	
