# V hlavickovych souborech MUSI být guardy!
# Dejte tam vsechny deklarace patrici do rozhrani.
# -Wall -Wextra -Werror -pedantic
CC=gcc
CFLAGS=-std=gnu99 
OBJ=main.o stack.o errors.o 

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -o proj

clean:
	rm  *.o proj

