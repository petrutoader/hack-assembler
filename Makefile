CC=gcc
CFLAGS=-I.

make:
	$(CC) -o bin/asm asm.c
