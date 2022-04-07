#include "grep.h"

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc)
{
    // TODO: fix this
    size_t i, j = 0;//1 
    for (i = 0; i < argc; i++) //1
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'o' || argv[i][j] == 'r' || argv[i][j] == 'n' 
            || argv[i][j] == 'c' || argv[i][j] == 'u' || argv[i][j] == 'M')
            {
                if (argv[i][j] == 'o')
                    opt->oflag = true;
                if (argv[i][j] == 'r')
                    opt->rflag = true;
                if (argv[i][j] == 'n')
                    opt->nflag = true;
                if (argv[i][j] == 'c')
                    opt->cflag = true;
                if (argv[i][j] == 'u')
                    opt->uflag = true;
                if (argv[i][j] == 'M')
                    opt->Mflag = true;

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



/**
** @brief               grep main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int grep(char** argv, BuiltinFd *builtinFd)
{
    size_t bufferSize = BUFFER_SIZE;
    Options opt;
    opt.cflag = false; 
    opt.hflag = false; 
    opt.iflag = false; 
    opt.lflag = false; 
    opt.nflag = false; 
    opt.vflag = false; 
    opt.eflag = false; 
    opt.ecount = -1; 
    opt.patterns = calloc(bufferSize, sizeof(char*));
    for (size_t i = 0; i < bufferSize; i++)
        opt.patterns[i] = calloc(bufferSize, sizeof(char)); 
    opt.fflag = false; 
    opt.filename = NULL; 
    opt.wflag = false; 
    opt.oflag = false; 
    opt.Aflag = false; 
    opt.Bflag = false; 
    opt.Cflag = false; 
    opt.n = -1;;
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err,"grep: usage: grep [options] pattern [files]\n");
        return -1;
    }
    else
    {
        getOptions(argv, &opt, &bufferSize, argc);
        
        // TODO:
    }


    for (size_t i = 0; i < bufferSize; i++)
        free(opt.patterns[i]);
    free(opt.patterns);
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
    grep(argv,terminal);
    free(terminal);
}