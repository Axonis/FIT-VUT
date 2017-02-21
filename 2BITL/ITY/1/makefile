#!/bin/sh
 
# Projekt 1 - ITY
# Autor: Jozef Urbanovský
# Login: xurban66
# Datum: 21.2.2017

CC = latex
NAME = proj1
PS = dvips -t a4
PDF = ps2pdf

clean: compile
	rm -f $(NAME).aux $(NAME).log  $(NAME).ps  $(NAME).dvi $(NAME).toc

compile:
	$(CC) $(NAME).tex
	$(CC) $(NAME).tex
	$(PS) $(NAME).dvi
	$(PDF) $(NAME).ps
