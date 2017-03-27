#!/bin/sh
 
# Projekt 2 - ITY
# Autor: Jozef Urbanovský
# Login: xurban66
# Datum: 8.3.2017

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
