CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1
LDLIBS = -fsanitize=address

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
MAIN = test.o


EXEC = test ls echo cat clear
all: $(EXEC)


test: $(MAIN) $(OBJ)
	$(CC) -g -o $@ $^

ls:
	$(CC) $(CFLAGS) -o $@ ./builtin/ls.c
echo:
	$(CC) $(CFLAGS) -o $@ ./builtin/echo.c ./builtin/builtin.c
cat:
	$(CC) $(CFLAGS) -o $@ ./builtin/cat.c
clear:
	$(CC) $(CFLAGS) -o $@ ./builtin/clear.c

$(OBJ): %.o: %.c $(HEADERS)

clean:
	$(RM) test *.o
	$(RM) ./builtin/test ./builtin/*.o
	$(RM) $(EXEC)