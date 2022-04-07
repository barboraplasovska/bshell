#include "cat.h"

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc)
{
    size_t i, j = 0;//1 
    for (i = 0; i < argc; i++) //1
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'A' || argv[i][j] == 'E' || argv[i][j] == 's'
		|| argv[i][j] == 'n')
            {
                if (argv[i][j] == 'A')
                    opt->Aflag = true;
                if (argv[i][j] == 's')
                    opt->sflag = true;
                if (argv[i][j] == 'E')
                    opt->Eflag = true;
                if (argv[i][j] == 'n')
                    opt->Eflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }
  if (i < argc)
  	opt->ind = i;
}

void getOperators(char** argv, Operators* ope, size_t argc)
{
   size_t i = 0;
   for (i = 0; i < argc; i++)
   {
	if (argv[i] == ">>")
	{
	   ope->doublesm = true;
	   break;
	}
	if (argv[i] == ">")
	{
	   ope->sm = true;
	   break;
	}
	if (argv[i][0] == '*')
	{
	   ope->star = true;
	   break;
	}
   }

   if (i < argv)
      ope->ind = i;
}

int infiniteCat (char** argv, BuiltinFd *builtinFd)
{
    return 0;
}

int singleFile (char* path, BuiltinFd *builtinFd, Options opt)
{
    if (opt.ind == -1)
    {
        int f;
        int count;
        char buffer[BUFFER_SIZE*3]; //characer buffer to store the bytes
        f = open(path, O_RDONLY);
        if(f == -1)
        {
            fprintf(builtinFd->err, "cat: error: cannot open file");
            return -1;
        }
        while((count = read(f, buffer, sizeof(buffer))) > 0)
        {
            fprintf(builtinFd->out, "%s", buffer);
        }
    }
    else
    {
        if (opt.nflag)
        {
            size_t i = 0;
            int f;
            int count;
            char buffer[BUFFER_SIZE*3]; //characer buffer to store the bytes
            f = open(path, O_RDONLY);
            if(f == -1)
            {
                fprintf(builtinFd->err, "cat: error: cannot open file");
                return -1;
            }
            while((count = read(f, buffer, sizeof(buffer))) > 0)
            {
                fprintf(builtinFd->out, "%lu: %s", i, buffer);
                i++;
            }
        }
        else if (opt.sflag)
        {
            
            size_t i = 0;
            int f;
            int count;
            char buffer[BUFFER_SIZE*3]; //characer buffer to store the bytes
            f = open(path, O_RDONLY);
            if(f == -1)
            {
                fprintf(builtinFd->err, "cat: error: cannot open file");
                return -1;
            }
            while((count = read(f, buffer, sizeof(buffer))) > 0)
            {
                if (buffer == '\n')
                {
                    if (i == 1)
                    {
                        continue;
                    }
                    i++;
                }
                else
                    i = 0;
                fprintf(builtinFd->out, "%s", buffer);
            }
        }
        /*else if (opt.Aflag)
        {
            // TODO
        }
        else if (opt.Eflag)
        {
            // TODO
        }*/
    }
     return 0;
}

int multipleFiles (char** argv, BuiltinFd *builtinFd, Options opt, size_t argc, 
    Operators ope)
{
    size_t i = 0;
    while (i < argc)
    {
        if (singleFile(argv[i], builtinFd, opt) == -1)
            return -1;
        i++;
    }
    return 0;
}

int appendFiles(char* source, char* destination, BuiltinFd *builtinFd)
{
    FILE *fp1;
    FILE *fp2;
 
    fp1 = fopen(source, "a+");
    fp2 = fopen(destination, "a+");
 
    if (!fp1 && !fp2) 
    {
        fprintf(builtinFd->err, "Unable to open/detect file(s)\n");
        return -1;
    }
 
    char buf[100];
 
    fprintf(fp2, "\n");
 
    while (!feof(fp1))
    {
        fgets(buf, sizeof(buf), fp1);
        fprintf(fp2, "%s", buf);
    }
 
    rewind(fp2);
 
    while (!feof(fp2)) 
    {
        fgets(buf, sizeof(buf), fp2);
        printf("%s", buf);
    }

    return 0;
}


int withOpe(char** argv, BuiltinFd *builtinFd, Operators ope, size_t argc)
{
    // no options, only operators
    size_t i = ope.ind;
    if (ope.sm == true)
    {
        if (i == 1)
        {
            if(argv[i] == NULL)
            {
                fprintf(builtinFd->err, "cat: missing file name.");
                return -1;
            }
            else
            {
                FILE *fp;
                if (access(argv[i], F_OK) == -1)  //if it doesn't exist
                {
                    fp = fopen(argv[i], "w");

                    if  (fp == NULL)
                    {
                        fprintf(builtinFd->err, "cat: Failed to create file\n");
                        return -1;
                    }

                    fclose(fp);
                }
            }
        }
        else if (i == 2)
        {
            if (argv[i] == NULL)
            {
                fprintf(builtinFd->err, "cat: missing file name.");
                return -1;
            }
            else
            {
                // copy files from 0 to i-2 to file i
                for (size_t j = 0; j < i-1; j++)
                {
                    if (appendFiles(argv[j], argv[i], builtinFd) == -1)
                        return -1;
                }
            }
        }
    }
    else if (ope.doublesm == true)
    {
        if (i == 0 || argv[0] == NULL || argv[1] == NULL)
        {
            fprintf(builtinFd->err, "cat: missing file(s) name\n");
            return -1;
        }

        if (appendFiles(argv[0], argv[2], builtinFd) == -1)
            return -1;
    }
    else if (ope.star == true)
    {
        // idk if it works
        char str[50];
        size_t i = 0;
        for (char c = argv[i-1][0]; c < strlen(argv[i-1]); i++)
        {
            if (i > 1)
            {
                str[i-2] = c;
            }
            i++;
        }


        DIR *di;
        char *ptr1,*ptr2;
        int retn;
        struct dirent *dir;
        di = opendir(".");
        if (di)
        {
            while ((dir = readdir(di)) != NULL)
            {
                ptr1=strtok(dir->d_name,".");
                ptr2=strtok(NULL,".");
                if(ptr2!=NULL)
                {
                    retn=strcmp(ptr2,str);
                    if(retn==0)
                    {
                        printf("%s\t",ptr1);
                    }
                }
            }
            closedir(di);
        }
    }
}

/**
** @brief               cat main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cat(char** argv, BuiltinFd *builtinFd)
{
    Options opt;
    opt.nflag = false;
    opt.sflag = false;
    opt.Aflag = false; // option A end E are illegal ??
    opt.Eflag = false;
    opt.ind = -1;

    Operators  ope;
    ope.sm = false;
    ope.doublesm = false;
    ope.star = false;
    ope.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        if (infiniteCat(argv, builtinFd) == -1)
            return -1;
    }
    else if (argc == 1)
    {
        if (singleFile(argv[1], builtinFd, opt) == -1)
            return -1;
    }
    else // more than 1 argument
    {
        getOptions(argv, &opt, argc);
        getOperators(argv, &ope, argc);

        if (opt.ind == -1)
        {
            // no options
            if (ope.ind == -1)
            {
                // no operators
                if (multipleFiles(argv, builtinFd, opt, argc, ope) == -1)
                    return -1;
            }
            else
            {
                // no options but operators
                if (withOpe(argv, builtinFd, ope, argc) == -1)
                    return -1;
            }
        }
        else
        {
            // options
            if (multipleFiles(argv, builtinFd, opt, argc, ope) == -1)
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
    cat(argv,terminal);
    free(terminal);
}
