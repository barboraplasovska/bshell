#include "exec.h"

/**
** @brief               Runs exec on argument
** @param argv          parameter.
** @param builtinFd     Files.
** @return              -1 on failure, 0 on success
*/
int exec(char **argv, BuiltinFd *builtinFd)
{
    /*if (!argv[0])
    {
        fprintf(builtinFd->err, "Exec: No arguments found!");
        return -1;
    }*/

    int pid = fork();
    if (pid == -1)
        return -1;
    if (!pid)
    {
        if (execvp(*argv, argv) == -1)
            errx(127, "fail");
    }
    else
    {
        int wstatus;
        if (waitpid(pid, &wstatus, 0) == -1)
            errx(0, "fail_p");
        if (WEXITSTATUS(wstatus) == 127)
            errx(1, "fail_p127");
        if (WEXITSTATUS(wstatus) == 2)
            errx(1, "syntax error (exit status 2)");
        fprintf(builtinFd->out, "exited with %d!\n", WEXITSTATUS(wstatus));
    }

    return 0;
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
    AppendToHistory(argv, "exec", terminal);

    if (argc){}

    if (exec(argv + 1, terminal) == -1)
    {
        // failed
    }

    return 0;
}
