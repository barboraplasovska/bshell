#include "cd.h"

/**
** @brief               cd main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cd(char** argv, BuiltinFd *builtinFd)
{
    //char *currDir;
    char *homeDir;
    char *dir;
    
    if ((homeDir = getenv("HOME")) == NULL)
        homeDir = "/";

    if (argv[0] == NULL)
        dir = homeDir;
    else
        dir = argv[1];

    //getcwd(currDir, BUFFER_SIZE);
    
    if (chdir(dir) == 0)
    {
        setenv("PWD", dir, 1);
        return 0;
    }
    
    fprintf(builtinFd->err, "cd: %s: No such file or directory\n", argv[0]);
    return 1;
}