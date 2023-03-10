CC = gcc
CFLAGS = -Wall -g -std=c90

calc: calculadora.o libpilha.o
	$(CC) calculadora.o libpilha.o -o calc -lm

calculadora.o : libpilha.h calculadora.c	
	$(CC) $(CFLAGS) -c calculadora.c

libpilha.o : libpilha.h libpilha.c
	$(CC) $(CFLAGS) -c libpilha.c

clean :
	rm -f *.o calc
