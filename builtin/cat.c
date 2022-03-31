#include "cd.h"

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
	if (argv[i] == "--")
	{
	   ope->dash = true;
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

int singleFile (char* path, BuiltinFd *builtinFd)
{
     int f;
     int count;
     char buffer[2048]; //characer buffer to store the bytes
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

     return 0;
}

int multipleFiles (char** argv, size_t argc, BuiltinFd *builtinFd)
{
     size_t i = 0;
     while (i < argc)
     {
	if (singleFile(argv[i], builtinFd) == -1)
	    return -1;
	i++;
     }
     return 0;
}

int withOpe(char** argv, BuiltinFd *builtinFd, Operators ope, size_t argc)
{
    // no options, only operators
    if (ope.sm == true)
    {
    }
    else if (ope.doublesm == true)
    {
    }
    else if (ope.dash == true)
    {
    }
    else if (ope.star == true)
    {
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
    opt.Aflag = false;
    opt.Eflag = false;
    opt.ind = -1;

    Operators  ope;
    ope.sm = false;
    ope.doublesm = false;
    ope.dash = false;
    ope.star = false;
    ope.ind = 0;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
	if (infiniteCat(argv, builtinFd) == -1)
		return -1;
    }
    else if (argc == 1)
    {
	if (singleFile(argv[1], builtinFd) == -1)
	    return -1;
    }
    else // more than 1 argument
    {
	getOptions(argv, &opt, argc);
	getOperators(argv, &ope, argc);

	if (opt.ind == -1)
	{
	    // no options
	    if (ope/ind == -1)
	    {
		// no operators
                if (multipleFiles(argv, argc, builtinFd) == -1)
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
	    if (withOpt(argv, builtinFd, opt, argc) == -1)
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
