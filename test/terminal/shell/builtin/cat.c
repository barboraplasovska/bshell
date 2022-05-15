#include "cd.h"

/**
** @brief               cat main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cat(char** argv, BuiltinFd *builtinFd)
{
    // infinite loop cat
    // if (argv[0] == NULL) {} else

    // single argument
    if (argv[2] == NULL)
    {
        int f;
        int count;
        char buffer[2048]; //characer buffer to store the bytes
        f = open(argv[1], O_RDONLY);
        if(f == -1)
        {
            fprintf(builtinFd->err, "cat: error: cannot open file\n");
            exit(EXIT_FAILURE);
            return -1;
        }
        while((count = read(f, buffer, sizeof(buffer))) > 0)
        {
            fprintf(builtinFd->out, "%s", buffer);
        }
    }
    else // multiple arguments
    {
        int f;
        int count;
        size_t i = 0;
        char buffer[2048]; //characer buffer to store the bytes
        while (argv[i] != NULL)
        {
            f = open(argv[i], O_RDONLY);
            if(f == -1)
            {
                fprintf(builtinFd->err, "cat: error: cannot open file\n");
                exit(EXIT_FAILURE);
                return -1;
            }
            while((count = read(f, buffer, sizeof(buffer))) > 0)
            {
                fprintf(builtinFd->out, "%s", buffer);
            }
            i++;
        }
    }
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
    int res = cat(argv,terminal);
    free(terminal);
    return res;
}
