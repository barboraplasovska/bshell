CC= gcc

CFLAGS= `pkg-config --cflags gtk+-3.0` -Wall -O3 `pkg-config --cflags --libs gtksourceview-4`
CPPLAGS= 
LDFLAGS= 

LDLIBS= `pkg-config --libs gtk+-3.0` `pkg-config --cflags --libs gtksourceview-4`

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)


run: $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -o run

debug: $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -g -fsanitize=address -o debug


.PHONY: clean

clean:
	rm -f run
	rm -f debug
	rm -f *.o
