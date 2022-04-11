CC= gcc

CFLAGS= -Wall -O3
CPPLAGS= 
LDFLAGS= 

LDLIBS=

SRC = $(wildcard bsh.c)
OBJ = $(SRC:.c=.o)


bsh: $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -o bsh

debug: $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -g -fsanitize=address -o bsh-debug


.PHONY: clean

clean:
	rm -f bsh-debug
	rm -f bsh
	rm -f *.o
