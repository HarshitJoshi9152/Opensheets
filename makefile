
CC=gcc
CFLAGS=-Wall -ggdb

compile: ./src/main.c ./src/*.h
	rm build -rf && mkdir build/
	$(CC) $(CFLAGS) src/main.c -o build/main.out -lm
