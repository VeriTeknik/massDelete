all: massDelete.c
	gcc -g -Wall -o massDelete massDelete.c

clean: 
	$(RM) massDelete