#include "date.h"

/**
** @brief               Gives the current system date.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int date(BuiltinFd *builtinFd)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(builtinFd->out, "%s", asctime (timeinfo));
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

    if (argc){}
    if (argv[1] == NULL)
    {
        date(terminal);
    }
    else
    {
        fprintf(terminal->err, "date: Invalid number of arguments\n");
    }
    free(terminal);
}
