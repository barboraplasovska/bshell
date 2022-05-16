#include "clearAnimations.h"


/**
** @brief               clear animations.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int clearAnimations(BuiltinFd *builtinFd)
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
            char *filePath = malloc(pathLength);
            strncpy(filePath, path, pathLength);
            if (c != '.')
            {
                size_t commandLength = 23 + strlen(dir->d_name) + 1;
                char* command = malloc(commandLength);
                strncpy(command, "./bshl removeanimation ", 24);

                for (size_t i = 23; i < commandLength - 1; i++)
                {
                    command[i] = dir->d_name[i - 23];
                }
                command[commandLength - 1] = '\0';

                //printf("run command:%s\n", command);

                system(command);
            }
        }
        closedir(d);
    }
    else
    {
        fprintf(builtinFd->err, "clearAnimations: Error reading directory, check permissions!\n");
        return -1;
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
    AppendToHistory(argv, "clearanimations", terminal);

    if (argv[0] != NULL)
    {
        fprintf(terminal->err, "clearanimations: This function doesn't take any parameters.\n");
        return -1;
    }

    clearAnimations(terminal);

    free(terminal);

    return 0;
}
