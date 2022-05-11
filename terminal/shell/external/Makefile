CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1

procStatus: procStatus.c
	$(CC) $(CFLAGS) -o procStatus procStatus.c

clean:
	${RM} -f *.o
	${RM} -f procStatus
