#include "cp.h"

int createFile(char* source, char* target, BuiltinFd *builtinFd)
{
   char ch;
   FILE *sourceF, *targetF;

   sourceF = fopen(source, "r");
   if (sourceF == NULL) 
   {
      fprintf(builtinFd->err, "mv: error opening source file");
      return -1;
   }
   targetF = fopen(target, "w");
   if (targetF == NULL) 
   {
      fclose(sourceF);
      fprintf(builtinFd->err, "mv: error opening source file");
      return -1;
   }

   while ((ch = fgetc(sourceF)) != EOF)
      fputc(ch, targetF);
    
   fclose(sourceF);
   fclose(targetF);

   return 0;
}

/**
** @brief               cp main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cp(char** argv, BuiltinFd *builtinFd)
{
    if (argv[0] == NULL || argv[1] == NULL)  //if cp has less than 2 arguments
    {
        return -1;
    }
    else
    {
        //deal with options later
        struct stat path_stat;
        int exit_code = stat(argv[0], &path_stat);

        int param1Type = -1;  //1->file; 2->directory
        if (S_ISREG(path_stat.st_mode)) 
        {
            param1Type = 1;
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            param1Type = 2;
        }
        
        exit_code = stat(argv[1], &path_stat);

        int param2type = = -1;  //1->file; 2->directory
        if (S_ISREG(path_stat.st_mode)) 
        {
            param2Type = 1;
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            param2Type = 2;
        }

        if(argv[2] == NULL)  //if we don't have a third argument
        {
            //case: 2 params
            // copy source text (file)->destination text(file)
            // copy source (file)->destination(directory)

            if(param1Type == 1 && param2type == 1)  //file and file
            {
                FILE *source, *target;
                source = fopen(source_file, "r");

                if (source == NULL)
                {
                    //error
                    return -1;
                }

                target = fopen(target, "w");

                if(target == NULL)
                {
                    //error
                    return -1;
                }

                char c;

                while((c = fgetc(source)) != EOF)
                    fputc(c, target);
                
                fclose(source);
                fclose(target);

            }
            else if (param1Type == 1 && param2type == 2)  //file and directory
            {
                //do mv here
                char *newPath = malloc (strlen(argv[0] + strlen(argv[1] + 2)));
                strcat(newPath, argv[0]);
                strcat(newPath, "/");
                strcat(newPath, argv[1]);
                if (createFile(argv[0], newPath, builtinFd) != 0)
                {
                    return -1;
                }
            }
            else
            {
                //error
                return -1;
            }
        }

        int argc = 2;

        for(; argv[argc] != NULL; argc++) { }

        exit_code = stat(argv[argc - 1], &path_stat);

        if(exit_code == -1)
        {
            //printf(" Error occoured attempting to stat %s\n", filename);
        }

        if(!S_ISDIR(path_stat.st_mode))
        {
            fprintf(builtinFd->err, "cp: target '" + argv[argc - 1] + "' is not a directory");
        }
        
        /*
        case: > 2 params
        multiple source files -> destination(directory)
        exception: if among the source files there exists a directory, it still copies but says that directory is omitted with name
        */
        for(int i = 2; i < argc - 1; i++)
        {
            //if file doesnt exist: cp: cannot stat 'testdir1': No such file or directory
            if ((access(argv[i], "F_OK") != 0)
            {
                printf(builtinFd->err, "cp: cannot stat '" + argv[i]) + "': No such file or directory");
            }

            exit_code = stat(argv[i], &path_stat);

            if(exit_code == -1)
            {
                //printf(" Error occoured attempting to stat %s\n", filename);
            }

            if(S_ISREG(path_stat.st_mode))  //if is regular file
            {
                // move into destination directory

                char *newPath = malloc (strlen(argv[i] + strlen(argv[argc-1] + 2)));
                strcat(newPath, argv[i]);
                strcat(newPath, "/");
                strcat(newPath, argv[argc-1]);
                if (createFile(argv[i], newPath, builtinFd) != 0)
                {
                    return -1;
                }

            }
            else if(S_ISDIR(path_stat.st_mode)) // if directory and exists
            {
                printf(builtinFd->err, "cp: -r not specified; omitting directory '" + argv[i]) + "'");
            }
        }

        return 0;
    }
}

int main(int argc, char **argv)
{
    if (argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    cp(argv,terminal);
    free(terminal);
}