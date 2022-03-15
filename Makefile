CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 -g
LDLIBS = -fsanitize=address
LDLFLAGS = -D_XOPEN_SOURCE=700

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
MAIN = tests.o


EXEC = test ls echo cat clear cd cp mv touch procstatus
all: $(EXEC)


test: $(MAIN) $(OBJ)
	$(CC) -g -o $@ $^

#BUILTIN
ls:
	$(CC) $(CFLAGS) -o $@ ./builtin/ls.c
echo:
	$(CC) $(CFLAGS) -o $@ ./builtin/echo.c ./builtin/builtin.c
cat:
	$(CC) $(CFLAGS) -o $@ ./builtin/cat.c
clear:
	$(CC) $(CFLAGS) -o $@ ./builtin/clear.c
cd:
	$(CC) $(CFLAGS) $(LDLFLAGS) -o $@ ./builtin/cd.c ./builtin/builtin.c
cp:
	$(CC) $(CFLAGS) -o $@ ./builtin/cp.c
mv:
	$(CC) $(CFLAGS) -o $@ ./builtin/mv.c ./builtin/builtin.c
touch:
	$(CC) $(CFLAGS) -o $@ ./builtin/touch.c

#EXTERNAL
procstatus:
	$(CC) $(CFLAGS) -o $@ ./external/procstatus.c

$(OBJ): %.o: %.c $(HEADERS)

clean:
	$(RM) test *.o
	$(RM) ./builtin/test ./builtin/*.o
	$(RM) $(EXEC)
