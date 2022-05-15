CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1

main:
	$(CC) -o main main.c parser.c parser.h

clean:
	$(RM) main
