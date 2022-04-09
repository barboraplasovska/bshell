CC= gcc

CFLAGS=  -Wall -O3 `pkg-config --cflags vte-2.91`
CPPLAGS= 
LDFLAGS= 

LDLIBS= `pkg-config --cflags vte-2.91` `pkg-config --libs vte-2.91`

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
