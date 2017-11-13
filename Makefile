# V hlavickovych souborech MUSI být guardy!
# Dejte tam vsechny deklarace patrici do rozhrani.

# Do proměnné OBJ přidejte vlastní název souboru.
# Pokud máte src.c i src.h, přidejte src.o. Pokud jen src.h, tak jen src.h.

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
OBJ=main.o lex.o strings.o errors.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -o proj

clean:
	rm  *.o proj

