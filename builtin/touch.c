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
            // TODO: create a clone if the name is a directory but the clone as a file
            if (access(argv[i], F_OK) == -1)  //if it doesn't exist
            {
                fp = fopen(argv[i], "w");

                if  (fp == NULL)
                {
                    fprintf(builtinFd->err, "Touch: Failed to create file\n");
                    //global variable errno is the error number
                    return -1;
                }

                fclose(fp);
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
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    touch(argv,terminal);
    free(terminal);
}
