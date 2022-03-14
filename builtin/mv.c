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

int mvMore2(size_t argc, char** argv, BuiltinFd *builtinFd)
{
    struct stat path_stat1;
    int err = stat(argv[argc-1], &path_stat1);
    if (err == -1)
    {
        fprintf(builtinFd->err, "mv: Error while stat (1).");
        return -1;
    }

    if (!(S_ISDIR(path_stat1.st_mode)))
    {
        fprintf(builtinFd->err, "mv: %s is not a directory", argv[argc-1]);
        return -1;
    }
    else
    {
        for (size_t i = 0; i < argc-1; i++)
        {
            err = stat(argv[i], &path_stat1);
            if (err == -1)
            {
                fprintf(builtinFd->err, "mv: Error while stat (2).");
                return -1;
            }
            char *newPath = malloc (strlen(argv[0] + strlen(argv[1] + 2)));
                strcat(newPath, argv[0]);
                strcat(newPath, "/");
                strcat(newPath, argv[1]);

            if (S_ISDIR(path_stat1.st_mode))
            {
                if (rename(argv[0], newPath) != 0)
                {
                    fprintf(builtinFd->err, "mv: rename error");
                    return -1;
                }
            }
            else
            {
                // moving file to a directory
                if (createFile(argv[0], newPath, builtinFd) != 0)
                {
                    return -1;
                }

            }
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
        fprintf(builtinFd->err, "mv: Error while stat (4).");
        return -1;
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
                strcat(newPath, argv[0]);
                strcat(newPath, "/");
                strcat(newPath, argv[1]);


                if (rename(argv[0], newPath) != 0)
                {
                    fprintf(builtinFd->err, "mv: rename error");
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
            strcat(newPath, argv[0]);
            strcat(newPath, "/");
            strcat(newPath, argv[1]);

            // move argv[0] to argv[1]
            if (createFile(argv[0], newPath, builtinFd) != 0)
            {
                return -1;
            }
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
            return -1;
    }
    else
    {
        if (mv2(argv, builtinFd) == -1)
            return -1;
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
    mv(argv,terminal);
    free(terminal);
}
