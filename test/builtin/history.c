#include "history.h"
#define _GNU_SOURCE

/**
** @brief               goes through the history of the commands and allows to call them
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
void history(BuiltinFd *builtinFd);
{
    
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 0)
        return 0;
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;

    argv=argv;
    history(terminal);

    free(terminal);
}
