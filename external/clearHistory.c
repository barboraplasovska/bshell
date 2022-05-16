#include "clearHistory.h"
#define _GNU_SOURCE

int main(int argc, char **argv)
{
    if (argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin; //STDIN_FILENO;
    terminal->out = stdout; //(FILE *) STDOUT_FILENO;
    terminal->err = stderr; //(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    
    if (*argv[0] == '\0')
    {
    	remove("myHistory.txt");
    }
    else
    {
	fprintf(terminal->err, "clearHistory: This function doesn't take any arguments.\n");
    }
    free(terminal);
}
