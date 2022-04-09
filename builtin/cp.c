#include "cp.h"

bool isDirEmpty(char *dirname)
{
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(dirname);

  if (dir == NULL)
    return true;
  while ((d = readdir(dir)) != NULL) 
  {
      if(++n > 2)
        break;
  }
  closedir(dir);

  if (n <= 2)
    return true;
  else
    return false;
}

int createFile(char* source, char* target, BuiltinFd *builtinFd)
{
   char ch;
   FILE *sourceF;
   FILE *targetF;

   sourceF = fopen(source, "r"); //r
   if (sourceF == NULL)
   {
      fprintf(builtinFd->err, "mv: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
      return -1;
   }
   targetF = fopen(target, "w+");
   /*if (targetF == NULL) 
   {
      fclose(sourceF);
      fprintf(builtinFd->err, "path: %s\n", target);
      fprintf(builtinFd->err, "mv: %s\n", strerror(errno));
      return -1;
   }*/

   while ((ch = fgetc(sourceF)) != EOF)
      fputc(ch, targetF);
    
   fclose(sourceF);
   fclose(targetF);

   return 0;
}

int mv2(char** argv, BuiltinFd *builtinFd)
{
    struct stat path_stat1;
    int err = stat(argv[0], &path_stat1);
    if (err == -1)
    {
        fprintf(builtinFd->err, "cp: Error while stat (3).");
        exit(EXIT_FAILURE);
        return -1;
    }

    struct stat path_stat2;
    err = stat(argv[1], &path_stat2);
    if (err == -1)
    {
        // second arg doesnt exist
        if (rename(argv[0], argv[1]) != 0)
        {
             fprintf(builtinFd->err, "cp: rename error");
             exit(EXIT_FAILURE);
             return -1;
        }

        return 0;
    }

    if (S_ISDIR(path_stat1.st_mode))
    {
        if (S_ISREG(path_stat2.st_mode))
        {
            fprintf(builtinFd->err, "cp: Illegal operation.");
            exit(EXIT_FAILURE);
            return -1;
        }
        else if (S_ISDIR(path_stat2.st_mode))
        {
            if (isDirEmpty(argv[1]) == true)
            {
                if (rename(argv[0], argv[1]) != 0)
                {
                    fprintf(builtinFd->err, "cp: rename error");
                    exit(EXIT_FAILURE);
                    return -1;
                }
            }
            else
            {
                char *newPath = malloc (strlen(argv[0] + strlen(argv[1] + 2)));
                strcat(newPath, argv[1]);
                strcat(newPath, "/");
                strcat(newPath, argv[0]);

                if (rename(argv[0], newPath) != 0)
                {
                    fprintf(builtinFd->err, "cp: rename error");
                    exit(EXIT_FAILURE);
                    return -1;
                }
            }
        }
    }
    else if (S_ISREG(path_stat1.st_mode))
    {
        if (S_ISDIR(path_stat2.st_mode))
        {
            char *newPath = malloc (strlen(argv[0] + strlen(argv[1] + 2)));
            strcat(newPath, argv[1]);
            strcat(newPath, "/");
            strcat(newPath, argv[0]);

            // move argv[0] to argv[1]
            if (createFile(argv[0], newPath, builtinFd) != 0)
            {
                exit(EXIT_FAILURE);
                return -1;
            }

	    //if (remove(argv[0]) != 0)
		//        return -1;

        }
        else if (S_ISREG(path_stat2.st_mode))
        {
            if (rename(argv[0], argv[1]) != 0)
            {
                fprintf(builtinFd->err, "cp: rename error");
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }
    else
    {
	if (rename(argv[0], argv[1]) != 0)
	{
	     fprintf(builtinFd->err, "cp: rename error");
         exit(EXIT_FAILURE);
	     return -1;
	}
    }

    return 0;
}

int mvMore2(size_t argc, char** argv, BuiltinFd *builtinFd) 
//if it is directory say omitting instead of moving,
{
    struct stat path_stat1;
    int err = stat(argv[argc-1], &path_stat1);
    if (err == -1)
    {
        fprintf(builtinFd->err,
                    "cp: target '%s' is not a directory\n",
                        argv[argc - 1]);
        exit(EXIT_FAILURE);
        return -1;
    }

    if (!(S_ISDIR(path_stat1.st_mode)))
    {
        fprintf(builtinFd->err, "cp: %s: is not a directory\n", argv[argc-1]);
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
	    char *newPath;
        for (size_t i = 0; i < argc-1; i++)
        {
            //fprintf(builtinFd->out, "ENTER\n");
            err = stat(argv[i], &path_stat1);
            if (err == -1)
            {
                fprintf(builtinFd->err,
                    "cp: cannot stat '%s': No such file or directory\n",
                        argv[i]);
                continue;
            }

            if(S_ISDIR(path_stat1.st_mode)) // if directory
            {
                fprintf(builtinFd->err, "cp: -r not specified; omitting directory '%s'\n", argv[i]);
                continue;
            }

            //fprintf(builtinFd->err, "argv[argc-1] = %s\n",argv[argc-1]);
            //fprintf(builtinFd->err, "argv[i] = %s\n",argv[i]);
            newPath = calloc (strlen(argv[i]) + strlen(argv[argc-1]) + 2,1);
                strcat(newPath, argv[argc-1]);
                strcat(newPath, "/");
                strcat(newPath, argv[i]);
            //fprintf(builtinFd->err, "path:'%s'\n", newPath);
            if (S_ISDIR(path_stat1.st_mode))
            {
                if (rename(argv[i], newPath) != 0)
                {
                    fprintf(builtinFd->err, "cp: rename error");
                    free(newPath);
                    exit(EXIT_FAILURE);
                    return -1;
                }
            }
            else
            {
                // moving file to a directory
                if (createFile(argv[i], newPath, builtinFd) != 0)
                {
                    free(newPath);
                    fprintf(builtinFd->out, "BREAK\n");
                    exit(EXIT_FAILURE);
                    return -1;
                }
            }
            free(newPath);
	    }
    }

    return 0;
}

/**
** @brief               cp main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cp(char** argv, int argc, BuiltinFd *builtinFd)
{
    if (argc == 0)
    {
        fprintf(builtinFd->err, "cp: missing file operand\n");
        fprintf(builtinFd->err, "Try 'cp --help' for more information.\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else if (argc == 1)
    {
        fprintf(builtinFd->err,
                "cp: missing destination file operand after '%s'\n", argv[0]);
        fprintf(builtinFd->err, "Try 'cp --help' for more information.\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        //deal with options later
        struct stat path_stat;
        stat(argv[0], &path_stat);

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
            exit(EXIT_FAILURE);
            return -1;
        }

        stat(argv[1], &path_stat);

        int param2type = -1;  //1->file; 2->directory
        if (S_ISREG(path_stat.st_mode)) 
        {
            param2type = 1;
        } 
        else if (S_ISDIR(path_stat.st_mode)) 
        {
            param2type = 2;
        }

        //fprintf(builtinFd->err, "We have: %i, and: %i\n", param1type, param2type);

        //if (argv[2])
        //{
        //    fprintf(builtinFd->err,"it thinks there is third arg: %s\n", argv[2]);
        //}

        if(argc == 2)  //if we don't have a third argument
        {
            //case: 2 params
            // copy source text (file)->destination text(file)
            // copy source (file)->destination(directory)

            if(param1type == 1 && param2type == 1)
            {
                //fprintf(builtinFd->err, "should create\n");
                //file1 and file2 where file2 already exists
                //file1 and file2 where file2 doesnt exist
                FILE *source, *target;
                source = fopen(argv[0], "r");

                //fprintf(builtinFd->err, "test!!\n");

                if (source == NULL)
                {
                    fprintf(builtinFd->err, "Cp: Check permissions..\n");
                    exit(EXIT_FAILURE);
                    return -1;
                }

                target = fopen(argv[1], "w");

                if(target == NULL)
                {
                    fprintf(builtinFd->err, "Cp: Check permissions..\n");
                    exit(EXIT_FAILURE);
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
                mv2(argv, builtinFd);
                return 0;
            }
        }
        
        /*
        case: > 2 params
        multiple source files -> destination(directory)
        exception: if among the source files there exists a directory, it still copies but says that directory is omitted with name or says can't stat when invalid
        */

        mvMore2(argc, argv, builtinFd);

        fflush(builtinFd->err);
        fflush(builtinFd->out);

        return 0;
    }
}

int main(int argc, char **argv)
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    int res = cp(argv, argc, terminal);
    free(terminal);
    return res;
}
