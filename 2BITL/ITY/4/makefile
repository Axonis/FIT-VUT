#!/bin/sh

# Projekt  - ITY proj4
# Autor: Jozef Urbanovský
# Login: xurban66
# Datum: 14.4.2017

	CC = latex
	BIB = bibtex
	PS = dvips -t a4
	BIBLIO = liter
	PDF= ps2pdf
	NAME = proj4

clean: compile
	rm -f $(NAME).aux $(NAME).log $(NAME).ps $(NAME).dvi $(NAME).bbl $(NAME).blg

compile:
	$(CC) $(NAME)
	$(BIB) $(NAME)
	$(CC) $(NAME)
	$(CC) $(NAME)
	$(PS) $(NAME).dvi 
	$(PDF) $(NAME).ps

