# V hlavickovych souborech MUSI být guardy!
# Dejte tam vsechny deklarace patrici do rozhrani.
# -Wall -Wextra -Werror -pedantic
CC=gcc 
#-D _GNU_SOURCE 
CFLAGS= -std=gnu99
OBJ=main.o garbage.o lex.o strings.o errors.o parser.o expresion.o code_gen_expres.c symbol.o symtable.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -o proj

clean:
	rm  *.o proj

