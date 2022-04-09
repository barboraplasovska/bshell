#include "builtin.h"

/**
** @brief               Gets the number of arguments.
** @param argv          Array of string arguments.
** @return              Returns the number of arguments.
*/
size_t getArgc(char **argv)
{
    size_t i = 0;
    while (argv[i] != NULL)
        i++;

    return i;
}

/**
** @brief               Appends the command to history file along with its args
** @param argv          argv passed in main
** @param builtinFd     Files.
** @return              Return 0 in case of success, -1 otherwise
*/
int AppendToHistory(char **argv, BuiltinFd *builtinFd)
{
    FILE *f;
    f = fopen("myHistory.txt", "a");
    if(f == NULL)
    {
        //perror("Error opening file.");
        fprintf(builtinFd->err ,"Failed to update history\n");
        return -1;
    }
    else
    {
        int i = 0;
        while (argv[i])
        {
            int j = 0;
            while (argv[i][j])
            {
                fputc(argv[i][j], f);
                j++;
            }
            fputc(' ', f);
            i++;
        }
    }
    fputc('\n', f);
    fclose(f);
    return 0;
}