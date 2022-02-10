#include <unistd.h>

/**
** @brief               touch main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int touch(char** argv, BuiltinFd *builtinFd)
{
    if(argv[0] == NULL)
    {
        //error
        fprintf(builtinFd->err, "Usage: touch [-c] [-d DATE] [-t DATE] [-r File] FILE...\n\n");
        fprintf(BuiltinFd->err, "Update the last-modified date on the given FILE[s]\n\n");
        fprintf("\t-c\tDon't create files");
        fprintf("\t-h\tDon't follow links");
        fprintf("\t-d DT\tDate/time to use");
        fprintf("\t-r FILE\tUse FILE's date/time'");
    }
    else
    {
        //deal with options later

        FILE *fp;
        for(int i = 0; argv[i] != NULL; i++)
        {
            if ((access(argv[i], "F_OK") != 0)  //if file doesn't exist
            {
                fp = fopen (argv[i], "r");
                fp.close();
            }
        }
    }
}