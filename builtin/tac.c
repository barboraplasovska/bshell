#include "tac.h"

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
            while (argv[i][j] == 's' || argv[i][j] == 'n' || argv[i][j] == 'b')
            {
                if (argv[i][j] == 'n')
                    opt->nflag = true;
                if (argv[i][j] == 's')
                    opt->sflag = true;
                if (argv[i][j] == 'b')
                    opt->bflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }
  if (i < argc)
  {
      opt->ind = i;
      if (opt->sflag)
      {
          opt->separator = argv[i];
          opt->ind += 1;
      }
  }
}

int singleFile (char* path, BuiltinFd *builtinFd, Options opt)
{
    if (opt.ind == -1)
    {
        // TODO:
    }
    else
    {
        if (opt.nflag)
        {
            // TODO:
        }
        else if (opt.sflag)
        {
            
            // TODO:
        }
        else if (opt.bflag)
        {
            // TODO:
        }
    }
     return 0;
}

int multipleFiles (char** argv, BuiltinFd *builtinFd, Options opt, size_t argc)
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

/**
** @brief               tac main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int tac(char** argv, BuiltinFd *builtinFd)
{
    Options opt;
    opt.nflag = false;
    opt.sflag = false;
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "tac: missing parameter\n");
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
        if (multipleFiles(argv, builtinFd, opt, argc) == -1)
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
    tac(argv,terminal);
    free(terminal);
}
