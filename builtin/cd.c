#include "cd.h"
#define _GNU_SOURCE

/**
** @brief               cd main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cd(char** argv, BuiltinFd *builtinFd)
{
    char *newPath;
    char *homeDir;
    char *dir;
    
    if ((homeDir = getenv("HOME")) == NULL)
        homeDir = "/";

    if (argv[0] == NULL)
        dir = homeDir;
    else
    {
	newPath = argv[0];
	//dir = getcwd(dir, BUFFER_SIZE);
        dir = getenv("PWD");
        strcat(dir, "/");
        strcat(dir, newPath);
    }
    //fprintf(builtinFd->out, "new path: %s\n", dir);
    int err = 0;
    if ((err = chdir(dir)) != 0)
    {
	fprintf(builtinFd->err, "cd: %d error\n", errno);
        fprintf(builtinFd->err, "cd: %s: No such file or directory\n", argv[0]);
	return 1;
    }
    //setenv("PWD", dir, 1);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 0)
        return 0;
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    cd(argv,terminal);
    free(terminal);
}
