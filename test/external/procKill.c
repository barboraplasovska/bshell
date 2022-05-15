#include "procKill.h"
#define _GNU_SOURCE

/**
** @brief               Kill process from pid.
** @param builtinFd     Files.
*/

void procKill(pid_t pid ,BuiltinFd *builtinFd)
{
    builtinFd = builtinFd;
    kill(pid, SIGKILL);
}

int main(int argc, char **argv)
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "prockill", terminal);

    if (argc){}
    if (argv[0] != NULL)
    {
        for (int i = 0; argv[i]; i++)
        {
            pid_t pid = atoi(argv[i]);
            procKill(pid, terminal);
        }
    }
    else
    {
        fprintf(terminal->err, "ProcKill: This function takes at least one argument!\n");
        return 1;
    }

    return 0;
}
