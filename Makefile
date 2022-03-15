CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1
LDLIBS = -fsanitize=address

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
MAIN = main.o


EXEC = main ls echo cat clear
all: $(EXEC)


main: $(MAIN) $(OBJ)
	$(CC) -g -o $@ $^

ls:
	$(CC) $(CFLAGS) -o $@ ./dir1/ls.c
echo:
	$(CC) $(CFLAGS) -o $@ ./dir1/echo.c ./dir1/builtin.c
cat:
	$(CC) $(CFLAGS) -o $@ ./dir1/cat.c
clear:
	$(CC) $(CFLAGS) -o $@ ./dir1/clear.c

$(OBJ): %.o: %.c $(HEADERS)

clean:
	$(RM) main *.o
	$(RM) ./dir1/main ./dir1/*.o
	$(RM) $(EXEC)
