CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

main: main.o expres.o 
	$(CC) $(CFLAGS) -o main main.o expres.o

main.o: main.c expres.c
	$(CC) $(CFLAGS) -c main.c

expres.o: expres.h

