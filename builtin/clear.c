#include "clear.h"

/**
** @brief               Clear main function.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int clear(BuiltinFd *builtinFd)
{
    fprintf(builtinFd->out, "'[H'[J");
    fflush(NULL);
    return 0;
}

int main()
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    clear(terminal);
    free(terminal);
}
