CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

# V hlavickovych souborech MUSI byt vsechny deklarace funkci a guardy!
# Dejte tam i dalsi deklarace patrici do rozhrani.
# Sem doplnte na oba radky "nazev souboru.o". soubor.o nahradte vasim a pridavejte dalsi. 
# Kazdy pouze jednou, bez pripony.
all: main.o lex.o strings.o symbol.o symtable.o
	$(CC) main.o lex.o strings.o symbol.o symtable.o -lm -o proj

clean:
	rm  *.o proj