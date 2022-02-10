#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
** @brief               cp main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cp(char** argv, BuiltinFd *builtinFd)
{
    if(argv[0] == NULL || argv[1] == NULL)  //if cp has less than 2 arguments
    {
        //error
    }
    else
    {
        //deal with options later

        for(int i = 0; argv[i] != NULL; i++)
        {
            struct stat path_stat;
            int exit_code = stat(argv[i], &path_stat);

            if(exit_code == -1)
            {
                //printf(" Error occoured attempting to stat %s\n", filename);
            }

            //case: 2 params
            // copy source (file)->destination(file)
            // copy source (file)->destination(directory)
            //case: > 2 params
            // multiple source files -> destination(directory)
            //exception: if among the source files there exists a directory, it still copies but says that directory is omitted with name

            if(S_ISREG(path_stat.st_mode))  //if is regular file
            {
                
            }
            else if(S_ISDIR(path_stat.st_mode))  // if is directory
            {
                
            }
        }
    }
}