# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
CC = gcc

all: tp3.o lib_racionais.o
	$(CC) -o tp3 tp3.o lib_racionais.o

lib_racionais.o: lib_racionais.c
	$(CC) -c $(CFLAGS) lib_racionais.c

tp3.o: tp3.c
	$(CC) -c $(CFLAGS) tp3.c

clean:
	rm -f *.o tp3
