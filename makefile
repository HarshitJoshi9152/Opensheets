
CC=gcc
CFLAGS=-Wall


compile: ./src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main.out
