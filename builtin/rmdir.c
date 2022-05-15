#include "rmdir.h"

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int rmDirs(char** argv, size_t argc, BuiltinFd* builtinFd)
{
    for (size_t i = 0; i < argc; i++)
    {
        if (isFile(argv[i]))
        {
            fprintf(builtinFd->err, "rmdir: Failed to remove '%s: Not a dir\n'",
                    argv[i]);
        }
        else
        {
            // check if directory exiasts
            DIR* dir = opendir(argv[i]);

            if (dir)
            {
                closedir(dir);
                if (rmdir(argv[i]) == -1)
                {
                    fprintf(builtinFd->err,
                            "rmdir: Failed to remove '%s': Directory not empty\n",
                            argv[i]);
                }
            }
            else if (ENOENT == errno)
            {
                fprintf(builtinFd->err, "rmdir: Directory '%s' doesn't exist\n",
                        argv[i]);
            }
            else
            {
                fprintf(builtinFd->err, "rmdir: Failed opening directory '%s'\n",
                        argv[i]);
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
        if (rmDirs(argv, argc, builtinFd) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
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
    rmdirr(argv,terminal);
    free(terminal);
}
