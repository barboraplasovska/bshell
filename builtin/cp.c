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
    if (argv[0] == NULL)
    {
        fprintf(builtinFd->err, "cp: missing file operand\n");
        fprintf(builtinFd->err, "Try 'cp --help' for more information.\n");
        return -1;
    }
    else if (argv[1] == NULL)
    {
        fprintf(builtinFd->err,
                "cp: missing destination file operand after '%s'\n", argv[0]);
        fprintf(builtinFd->err, "Try 'cp --help' for more information.\n");
        return -1;
    }
    else
    {
        //deal with options later
        struct stat path_stat;
        int exit_code = stat(argv[0], &path_stat);

        int param1type = -1;  //1->file; 2->directory
        if (S_ISREG(path_stat.st_mode)) 
        {
            param1type = 1;
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            param1type = 2;
        }
        else
        {
            fprintf(builtinFd->err,
                    "cp: cannot stat '%s': No such file or directory\n",
                    argv[0]);
        }

        exit_code = stat(argv[1], &path_stat);

        int param2type = -1;  //1->file; 2->directory
        if (S_ISREG(path_stat.st_mode)) 
        {
            param2type = 1;
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            param2type = 2;
        }

        fprintf(builtinFd->err, "We have: %i, and: %i\n", param1type, param2type);

        if(argv[2] == NULL)  //if we don't have a third argument
        {
            //case: 2 params
            // copy source text (file)->destination text(file)
            // copy source (file)->destination(directory)

            if(param1type == 1 && param2type == 1)
            {
                //file1 and file2 where file2 already exists
                //file1 and file2 where file2 doesnt exist
                FILE *source, *target;
                source = fopen(argv[0], "r");

                fprintf(builtinFd->err, "test!!\n");

                if (source == NULL)
                {
                    // failed to open file eventhough it exists! permissions..
                    return -1;
                }

                target = fopen(argv[1], "w");

                if(target == NULL)
                {
                    fprintf(builtinFd->err, "test..\n");
                    //failed to open file2 eventhough it exists! permissions..
                    return -1;
                }

                char c;

                while((c = fgetc(source)) != EOF)
                    fputc(c, target);
                
                fclose(source);
                fclose(target);

                return 0;
            }
            else if (param1type == 1 && param2type == 2)  //file and directory
            {
                //move file1 into dirfile2
                char *newPath = malloc (strlen(argv[0] + strlen(argv[1] + 2)));
                strcat(newPath, argv[0]);
                strcat(newPath, "/");
                strcat(newPath, argv[1]);
                if (createFile(argv[0], newPath, builtinFd) != 0)
                {
                    //failed to createfile! permissions..
                    return -1;
                }

                return 0;
            }
        }
        
        //FIX 
        /*
        case: > 2 params
        multiple source files -> destination(directory)
        exception: if among the source files there exists a directory, it still copies but says that directory is omitted with name
        */

	// fix this
	int argc = 2;
	// fix this

        for(int i = 2; i < argc - 1; i++)
        {
            //if file doesnt exist: cp: cannot stat 'testdir1': No such file or directory
            //if (access(argv[i], "F_OK") != 0)
            if (!opendir(argv[i]))
            {
                fprintf(builtinFd->err, "cp: cannot stat '%s': No such file or directory", argv[i]);
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
                fprintf(builtinFd->err, "cp: -r not specified; omitting directory '%s'", argv[i]);
            }
        }

        fflush(builtinFd->err);
        fflush(builtinFd->out);

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
    return 0;
}
