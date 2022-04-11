#include "touch.h"

/**
** @brief               touch main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int touch(char** argv, BuiltinFd *builtinFd)
{
    if(argv[0] == NULL)
    {
        fprintf(builtinFd->err, "touch: Not enough parameters.");
    }
    else
    {
        //deal with options later   
        FILE *fp;
        for(int i = 0; argv[i] != NULL; i++)
        {
            if (access(argv[i], "F_OK") != 0)  //if file doesn't exist
            {
                fp = fopen(argv[i], "r");

                if  (fp == NULL)
                {
                    //global variable errno is the error number
                    return -1;
                }

                fclose(fp);
            }       //else it does nothing as it should
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
    touch(argv,terminal);
    free(terminal);
}