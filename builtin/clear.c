#include "clear.h"

/**
** @brief               Clear main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int clear(char** argv, BuiltinFd *builtinFd)
{
    if (argv[0] != NULL)
    {
        fprintf(builtinFd->err, "clear: too many arguments");
        exit(EXIT_FAILURE);
        return -1;
    }
    fprintf(builtinFd->out, "\e[1;1H\e[2J");
    fflush(NULL);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "clear", terminal);
    int res = clear(argv, terminal);
    free(terminal);
    return res;
}
