#include "rmdir.h"

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int removeDir(char* path)
{
    struct dirent **namelist;
    int err = scandir(path, &namelist, NULL, alphasort);
    if (err == -1)
        return -1;
    
    for (size_t i = err-1; i > 0 ; i--)
    {
        char* name = path;
        strcat(name, namelist[i]->d_name);
        if (isFile(name))
        {
            if (remove(name) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        else
        {
            if (removeDir(name) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }

    // remove empty dir
    if (rmdir(path) == -1)
    {
        exit(EXIT_FAILURE);
        return -1;
    }

    return 0;
}

int rmDirs(char** argv, size_t argc)
{
    for (size_t i = 0; i < argc; i++)
    {
        if (isFile(argv[i]))
        {
            if (remove(argv[i]) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        else
        {
            if (removeDir(argv[i]) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }
    return 0;
}

/**
** @brief               rm main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int rmdirr(char** argv, BuiltinFd *builtinFd)
{
    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "rmdir: missing an argument\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        if (rmDirs(argv, argc) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }
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
    rmdirr(argv,terminal);
    free(terminal);
}