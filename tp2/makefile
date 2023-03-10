# Makefile do trabalho TP2

CFLAGS = -Wall -std=c90 -g
CC = gcc

tp2: tp2.o lib_racionais.o
	$(CC) -o tp2 tp2.o lib_racionais.o

lib_racionais.o: lib_racionais.c
	$(CC) -c $(CFLAGS) lib_racionais.c

tp2.o: tp2.c
	$(CC) -c $(CFLAGS) tp2.c

clean:
	rm -f *.o tp2
