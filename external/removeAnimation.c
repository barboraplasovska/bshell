#include "removeAnimation.h"

/**
** @brief               Remove an animation.
** @param name          Animation name.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int removeAnimation(char* name, BuiltinFd *builtinFd)
{
    size_t pathLength = 11 + strlen(name) + 1;
    char *path = malloc(pathLength);
    strncpy(path, "Animations/", 12);

    for (size_t i = 11; i < pathLength - 1; i++)
    {
        path[i] = name[i - 11];
    }
    path[pathLength - 1] = '\0';

    //remove all files inside path
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
            if (isdigit(c))
            {
                filePath = realloc(filePath, pathLength + 2);
                filePath[pathLength - 1] = '/';
                filePath[pathLength] = c;
                filePath[pathLength + 1] = '\0';
                //remove file here
                remove(filePath);
            }
        }
        //remove the directory here
        rmdir(path);
        closedir(d);
    }
    else if (ENOENT == errno)
    {
        // directory doesnt exist
        fprintf(builtinFd->err, "removeanimation: the directiory %s doesn't exist\n", path);
        return -1;
    }
    else
    {
        fprintf(builtinFd->err, "removeanimation: Failed opening directory, check permissions!\n");
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
    AppendToHistory(argv, "removeanimation", terminal);
    //printf("argv[0] = %s\n",argv[0]);
    //printf("argv[1] = %s\n",argv[1]);

    if (*argv[0] == '\0') //|| *argv[2] != '\0')
    {
        fprintf(terminal->err, "removeanimation: This function only takes 1 parameter\n");
        return -1;
    }

    removeAnimation(argv[0],terminal);

    free(terminal);

    return 0;
}
