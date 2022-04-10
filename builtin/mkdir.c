#include "mkdir.h"

/**
** @brief               mkdir main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int mkdirr(char** argv, BuiltinFd *builtinFd)
{
    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "mkdir: missing an argument\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        for (size_t i = 0; i < argc; i++)
        {
            struct stat st = {0};

            if (stat(argv[i], &st) == -1) 
            {
                mkdir(argv[i], 0700);
            }
            else
            {
                fprintf(builtinFd->err, "mkdir: %s: File exists\n", argv[i]);
                continue;
            }
        }
    }
    return 0;
}

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
    AppendToHistory(argv, "mkdir", terminal);
    mkdirr(argv,terminal);
    free(terminal);
}
