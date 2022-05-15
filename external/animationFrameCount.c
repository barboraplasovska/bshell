#include "animationFrameCount.h"

/**
** @brief               Counts number of framces in an animation
** @param name          Animation name.
** @param builtinFd     Files.
** @return              return 0 on success
*/
int countAnimationFrames(char* name, BuiltinFd *builtinFd)
{
    size_t count = 0;
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
            if (isdigit(c))
            {
                count++;
            }
        }
        closedir(d);

        fprintf(builtinFd->out, "There are %zu frames.\n", count);
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
    AppendToHistory(argv, "animationframecount", terminal);

    if (argv[1] != NULL)
    {
        fprintf(terminal->err, "animationframecount: This function only takes 1 parameter\n");
        return -1;
    }

    countAnimationFrames(argv[0],terminal);

    free(terminal);

    return 0;
}
