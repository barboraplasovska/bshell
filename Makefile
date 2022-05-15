CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g #-O1 -g
LDLIBS =  -pthread #-fsanitize=address -pthread
LDLFLAGS = -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
MAIN = test.o

EXEC = test alias cat clear cp date echo grep head help ls mkdir mv rm rmdir sort tac tail touch cpuinfo history prockill proclist procstatus clearhistory unalias exec createanimation removeanimation listanimations clearanimations animationframecount runanimation
all: $(EXEC)


test: $(MAIN) $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) -g -o $@ $^

#BUILTIN
alias:
	$(CC) $(CFLAGS) -o $@ ./builtin/alias.c ./builtin/builtin.c
cat:
	$(CC) $(CFLAGS) -o $@ ./builtin/cat.c ./builtin/builtin.c
clear:
	$(CC) $(CFLAGS) -o $@ ./builtin/clear.c ./builtin/builtin.c
cp:
	$(CC) $(CFLAGS) -o $@ ./builtin/cp.c ./builtin/builtin.c
date:
	$(CC) $(CFLAGS) -o $@ ./builtin/date.c ./builtin/builtin.c
echo:
	$(CC) $(CFLAGS) -o $@ ./builtin/echo.c ./builtin/builtin.c
grep:
	$(CC) $(CFLAGS) -o $@ ./builtin/grep.c ./builtin/builtin.c
head:
	$(CC) $(CFLAGS) -o $@ ./builtin/head.c ./builtin/builtin.c
help:
	$(CC) $(CFLAGS) -o $@ ./builtin/help.c
ls:
	$(CC) $(CFLAGS) -o $@ ./builtin/ls.c ./builtin/builtin.c
mkdir:
	$(CC) $(CFLAGS) -o $@ ./builtin/mkdir.c ./builtin/builtin.c
mv:
	$(CC) $(CFLAGS) -o $@ ./builtin/mv.c ./builtin/builtin.c
rm:
	$(CC) $(CFLAGS) $(LDLFLAGS) -o $@ ./builtin/rm.c ./builtin/builtin.c
rmdir:
	$(CC) $(CFLAGS) $(LDLFLAGS) -o $@ ./builtin/rmdir.c ./builtin/builtin.c
sort:
	$(CC) $(CFLAGS) $(LDLFLAGS) -o $@ ./builtin/sort.c ./builtin/builtin.c
tac:
	$(CC) $(CFLAGS) -o $@ ./builtin/tac.c ./builtin/builtin.c
tail:
	$(CC) $(CFLAGS) -o $@ ./builtin/tail.c ./builtin/builtin.c
touch:
	$(CC) $(CFLAGS) -o $@ ./builtin/touch.c ./builtin/builtin.c
unalias:
	$(CC) $(CFLAGS) -o $@ ./builtin/unalias.c ./builtin/builtin.c

#EXTERNAL
cpuinfo:
	$(CC) $(CFLAGS) -o $@ ./external/cpuInfo.c ./builtin/builtin.c
history:
	$(CC) $(CFLAGS) -o $@ ./external/history.c ./builtin/builtin.c
prockill:
	$(CC) $(CFLAGS) -o $@ ./external/procKill.c ./builtin/builtin.c
proclist:
	$(CC) $(CFLAGS) -o $@ ./external/procList.c ./builtin/builtin.c
procstatus:
	$(CC) $(CFLAGS) -o $@ ./external/procStatus.c ./builtin/builtin.c
clearhistory:
	$(CC) $(CFLAGS) -o $@ ./external/clearHistory.c ./builtin/builtin.c
exec:
	$(CC) $(CFLAGS) -o $@ ./external/exec.c ./builtin/builtin.c

#ANIMATIONS
createanimation:
	$(CC) $(CFLAGS) -o $@ ./external/createAnimation.c ./builtin/builtin.c
removeanimation:
	$(CC) $(CFLAGS) -o $@ ./external/removeAnimation.c ./builtin/builtin.c
listanimations:
	$(CC) $(CFLAGS) -o $@ ./external/listAnimations.c ./builtin/builtin.c
clearanimations:
	$(CC) $(CFLAGS) -o $@ ./external/clearAnimations.c ./builtin/builtin.c
animationframecount:
	$(CC) $(CFLAGS) -o $@ ./external/animationFrameCount.c ./builtin/builtin.c
runanimation:
	$(CC) $(CFLAGS) -o $@ ./external/runAnimation.c ./builtin/builtin.c


$(OBJ): %.o: %.c $(HEADERS)

clean:
	$(RM) test *.o
	$(RM) ./builtin/main ./builtin/*.o
	$(RM) $(EXEC)
	$(RM) myHistory.txt
