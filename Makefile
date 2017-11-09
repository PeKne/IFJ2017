CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

# V hlavickovych souborech MUSI byt guardy a pro jistotu bych tam dal i vsechny deklarace funkci. 
# Dejte tam i dalsi deklarace patrici do rozhrani.
# Sem doplnte na oba radky "nazev souboru.o". soubor.o nahradte vasim a pridavejte dalsi. 
# Kazdy pouze jednou, bez pripony.
all: main.o soubor.o
	$(CC) main.o soubor.o -lm -o proj

clean:
	rm  *.o proj

