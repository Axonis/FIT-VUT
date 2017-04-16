#!/bin/sh
 
# Projekt 2 - ITY
# Autor: Jozef Urbanovský
# Login: xurban66
# Datum: 8.3.2017

CC = pdflatex
NAME = proj5

clean: compile
	rm -f $(NAME).aux $(NAME).log  $(NAME).nav  $(NAME).snm $(NAME).toc $(NAME).out

compile:
	$(CC) $(NAME).tex
	$(CC) $(NAME).tex