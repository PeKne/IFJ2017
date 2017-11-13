CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

# V hlavickovych souborech MUSI být guardy!
# Dejte tam vsechny deklarace patrici do rozhrani.

# Sem doplnte na oba radky "nazev souboru.o". soubor.o nahradte vasim a pridavejte dalsi. 
# Kazdy pouze jednou, bez pripony.
all: main.o lex.o strings.o errors.h
	$(CC) main.o lex.o strings.o errors.h -lm -o proj

clean:
	rm  *.o proj

