#include "rm.h"


/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc)
{
    size_t i, j = 0;//1 
    for (i = 0; i < argc; i++) //1
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'i' || argv[i][j] == 'r' || argv[i][j] == 'f'
                || argv[i][j] == 'R')
            {
                if (argv[i][j] == 'i')
                    opt->iflag = true;
                if (argv[i][j] == 'r' || argv[i][j] == 'R')
                    opt->rflag = true;
                if (argv[i][j] == 'f')
                    opt->fflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }
  opt->ind = i;
}

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int removeDir(char* path, BuiltinFd* builtinFd)
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
            if (removeDir(name, builtinFd) == -1)
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

int rmDirs(char** argv, size_t argc, Options opt, BuiltinFd* builtinFd)
{
    for (size_t i = opt.ind; i < argc; i++)
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
            if (removeDir(argv[i], builtinFd) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }        
        }
    }
    return 0;
}

int rmFiles (char** argv, size_t argc, Options opt, BuiltinFd* builtinFd)
{
    if (opt.ind == -1)
    {
        for (size_t i = 0; i < argc; i++)
        {
            if (remove(argv[i]) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }
    else
    {
        for (size_t i = opt.ind; i < argc; i++)
        {
            if (isFile(argv[i]))
            {
                if (opt.iflag)
                {
                    FILE* fp = open(argv[i], O_RDONLY);
                    int c = fgetc(fp);
                    if (c == EOF)
                    {
                        // file empty
                        fprintf(builtinFd->out, 
                            "rm: remove regular empty file '%s'? y", argv[i]);
                    } 
                    else
                    {
                        fprintf(builtinFd->out, 
                            "rm: remove regular non-empty file '%s'? y", argv[i]);
                        ungetc(c, fp);
                    }
                    int c = getchar();
                    if (c == 'y')
                    {
                        if (remove(argv[i]) == -1)
                        {
                            exit(EXIT_FAILURE);
                            return -1;
                        }
                    }
                    else
                        continue;
                }
                else
                {
                    if (remove(argv[i]) == -1)
                    {
                        exit(EXIT_FAILURE);
                        return -1;
                    }
                }
            }
            else
            {
                fprintf(builtinFd->err, "rm: %s: is a directory\n", argv[i]);
                continue;
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
int rm(char** argv, BuiltinFd *builtinFd)
{
    size_t argc = getArgc(argv);
    struct Options opt;
    opt.iflag = false;
    opt.fflag = false;
    opt.rflag = false;
    opt.ind = -1;

    if (argc == 0)
    {
        fprintf(builtinFd->err, "rm: missing an argument\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        getOptions(argv, &opt, argc);

        if (opt.rflag)
        {
            if (rmDirs(argv, argc, opt, builtinFd) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        else
        {
            if (rmFiles(argv, argc, opt, builtinFd) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
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
    rm(argv,terminal);
    free(terminal);
}