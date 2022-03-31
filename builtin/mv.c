#include "mv.h"

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

   sourceF = fopen(source, "r");
   if (sourceF == NULL)
   {
      fprintf(builtinFd->err, "mv: %s\n", strerror(errno));
      return -1;
   }
   targetF = fopen(target, "w+");

   while ((ch = fgetc(sourceF)) != EOF)
      fputc(ch, targetF);
    
   fclose(sourceF);
   fclose(targetF);

   return 0;
}

int mvMore2(size_t argc, char** argv, BuiltinFd *builtinFd)
{
    struct stat path_stat1;
    int err = stat(argv[argc-1], &path_stat1);
    if (err == -1)
    {
        fprintf(builtinFd->err, "mv: Error while stat (1).\n");
        return -1;
    }

    if (!(S_ISDIR(path_stat1.st_mode)))
    {
        fprintf(builtinFd->err, "mv: %s: is not a directory\n", argv[argc-1]);
        return -1;
    }
    else
    {
	char *newPath;

        for (size_t i = 0; i < argc-1; i++)
        {
            err = stat(argv[i], &path_stat1);
            if (err == -1)
            {
                fprintf(builtinFd->err,
                        "mv: %s: file or directory doesn't exist.\n",
                          argv[i]);

                return -1;
            }

            newPath = calloc (strlen(argv[i]) + strlen(argv[argc-1]) + 2, 1);
            strcat(newPath, argv[argc-1]);
            strcat(newPath, "/");
            strcat(newPath, argv[i]);

            if (S_ISDIR(path_stat1.st_mode))
            {
                if (rename(argv[i], newPath) != 0)
                {
                    fprintf(builtinFd->err, "mv: rename error");
                    free(newPath);
                    return -1;
                }
            }
            else
            {
                // moving file to a directory
                if (createFile(argv[i], newPath, builtinFd) != 0)
                {
                    free(newPath);
                    return -1;
                }

		
	            if (remove(argv[i]) != 0)
	            {
		            free(newPath);
		            return -1;
	            }
            }
	    free(newPath);
        }
    }

    return 0;
}

int mv2(char** argv, BuiltinFd *builtinFd)
{
    struct stat path_stat1;
    int err = stat(argv[0], &path_stat1);
    if (err == -1)
    {
        fprintf(builtinFd->err, "mv: Error while stat (3).");
        return -1;
    }

    struct stat path_stat2;
    err = stat(argv[1], &path_stat2);
    if (err == -1)
    {
        // second arg doesnt exist
        if (rename(argv[0], argv[1]) != 0)
        {
             fprintf(builtinFd->err, "mv: rename error");
             return -1;
        }

        return 0;
    }

    if (S_ISDIR(path_stat1.st_mode))
    {
        if (S_ISREG(path_stat2.st_mode))
        {
            fprintf(builtinFd->err, "mv: Illegal operation.");
            return -1;
        }
        else if (S_ISDIR(path_stat2.st_mode))
        {
            if (isDirEmpty(argv[1]) == true)
            {
                if (rename(argv[0], argv[1]) != 0)
                {
                    fprintf(builtinFd->err, "mv: rename error");
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
                    fprintf(builtinFd->err, "mv: rename error");
                    free(newPath);
                    return -1;
                }

                free(newPath);
	            if (remove(argv[0]) != 0)
		            return -1;
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
                free(newPath);
                return -1;
            }

            free(newPath);

	        if (remove(argv[0]) != 0)
		        return -1;
        }
        else if (S_ISREG(path_stat2.st_mode))
        {
            if (rename(argv[0], argv[1]) != 0)
            {
                fprintf(builtinFd->err, "mv: rename error");
                return -1;
            }
        }
    }
    else
    {
	    if (rename(argv[0], argv[1]) != 0)
	    {
	        fprintf(builtinFd->err, "mv: rename error");
	        return -1;
	    }
    }

    return 0;
}

/**
** @brief               Move main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int mv(char** argv, BuiltinFd *builtinFd)
{
    size_t argc = getArgc(argv);

    if (argc == 0)
        fprintf(builtinFd->err, "mv: Missing arguments.");
    else if (argc > 2)
    {
        if (mvMore2(argc, argv, builtinFd) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }
    else
    {
        if (mv2(argv, builtinFd) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }

    return 0;
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
    int res = mv(argv,terminal);  //argv+1 to debug
    free(terminal);
    return res;
}
