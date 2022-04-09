#define _GNU_SOURCE
#include "ls.h"
#include <errno.h>
/**
** @brief               Ls main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int ls(char** argv, BuiltinFd *builtinFd)
{
    struct dirent **namelist;
    int err;

    if (argv[0] == NULL)
    {
        char currDir[BUFFER_SIZE];
        getcwd(currDir, BUFFER_SIZE);
        err = scandir(currDir, &namelist, NULL, alphasort);

        if (err == -1)
        {
            fprintf(builtinFd->err, "ls: Directory cannot be opened for reading.");
            return 1;
        }
    }
    else
    {
        err = scandir(argv[0], &namelist, NULL, alphasort);

        if (err == -1)
        {
            fprintf(builtinFd->err, "ls: %s: No such file or directory\n", argv[0]);
            return 1;
        }
    }

    for (int i = err-1; i -1 > 0; i--)
    {
        //if (i-1 >= 0)
            fprintf(builtinFd->out, "%s ", namelist[i]->d_name);
        //else
        //    fprintf(builtinFd->out, "%s ", namelist[i]->d_name);
    }
    fprintf(builtinFd->out, "\n");
    free(namelist);
    
    return 0;
}

int main(int argc, char **argv)
{
    if(argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;
    terminal->out = stdout;
    terminal->err = stderr;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    //printf("this is argv[0] : %s\n",argv[0]);
    ls(argv,terminal);
    free(terminal);
}
