#include "tail.h"

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
            while (argv[i][j] == 'n' || argv[i][j] == 'q' || argv[i][j] == 'v')
            {
                if (argv[i][j] == 'n')
                    opt->nflag = true;
                if (argv[i][j] == 'q')
                    opt->qflag = true;
                if (argv[i][j] == 'v')
                    opt->vflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }
  if (opt->nflag)
  {
      char* endptr;
      opt->nb = strtol(argv[i],&endptr  ,10);
      i++;
  }

  if (i < argc)
  	opt->ind = i;
}

char** getFileContent (char* path, size_t *length, size_t *bufferSize)
{
    char** lines = calloc(BUFFER_SIZE, sizeof(char*));

    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        lines[i] = calloc(BUFFER_SIZE, sizeof(char));
    }

    size_t nbLines = 0;
    size_t buffSize = BUFFER_SIZE;

    FILE* f = fopen(path, "r");
    while(fgets(lines[nbLines], BUFFER_SIZE, f)) 
    {
	    lines[nbLines][strcspn(lines[nbLines], "\n")] = '\0';
        nbLines++;
        if (nbLines >= buffSize)
        {
            buffSize *= 2;
            lines = realloc(lines, buffSize * sizeof(char*));
            for (size_t i = 0; i < buffSize; i++)
            {
                lines[i] = realloc(lines[i], buffSize * sizeof(char));
            }
        }
    }
    *length = nbLines;
    *bufferSize = buffSize;
    return lines;
}

int oneFile(char* path, Options opt, BuiltinFd* builtinFd)
{
    size_t length;
    size_t bufferSize;
    char** lines = getFileContent(path, &length, &bufferSize);

    if (opt.vflag)
        fprintf(builtinFd->out, "       ==> %s <== \n", path);

    size_t i = length;
    if (opt.nflag)
        i -= opt.nb;
    else
        i -= 10;
   
    for (; i < length; i++)
        fprintf(builtinFd->out, "%s\n", lines[i]);

    fflush(builtinFd->out);
    // freeeing lines
    for (size_t i = 0; i < bufferSize; i++)
    {
        free(lines[i]);
    }
    free(lines);

    return 0;
}

int multiFiles(char** argv, Options opt, size_t argc, BuiltinFd* builtinFd)
{
    opt.vflag = false;

    for (size_t i = opt.ind; i < argc; i++)
    {
        if (!opt.qflag)
            fprintf(builtinFd->out, "   %s\n", argv[i]);

        if (oneFile(argv[i], opt, builtinFd) != 0)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }
    return 0;
}

/**
** @brief               tail main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int tail(char** argv, BuiltinFd *builtinFd)
{
    Options opt;
    opt.nflag = false;
    opt.vflag = false;
    opt.qflag = false;
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "tail: missing parameter\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        getOptions(argv, &opt, argc);

        if (argc-opt.ind > 1) // multiple files
        {
            if (multiFiles(argv, opt, argc, builtinFd) != 0)
            {
		fflush(builtinFd->out);
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        else // one file
        {
	    if (opt.ind < 0)
		opt.ind = 0;
            if (oneFile(argv[opt.ind], opt, builtinFd) != 0)
            {
		fflush(builtinFd->out);
                exit(EXIT_FAILURE);
                return -1;
            }
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
    AppendToHistory(argv, "tail", terminal);
    tail(argv,terminal);
    free(terminal);
}
