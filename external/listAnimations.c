#include "listAnimations.h"


/**
** @brief               list animations.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int listAnimations(BuiltinFd *builtinFd)
{
    size_t pathLength = 11 + 1;
    char *path = malloc(pathLength);
    strncpy(path, "Animations/", 12);

    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char c = dir->d_name[0];
            if (c != '.')
            {
                //remove file here
                fprintf(builtinFd->out, "%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    else
    {
        fprintf(builtinFd->out, "listAnimations: failed opening directory...\n");
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
    AppendToHistory(argv, "listanimations", terminal);

    if (argv[0] != NULL)
    {
        fprintf(terminal->err, "listanimations: This function doesn't take any parameters.\n");
        return -1;
    }
    
    listAnimations(terminal);

    free(terminal);

    return 0;
}
