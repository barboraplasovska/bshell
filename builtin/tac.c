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
            while (argv[i][j] == 'n')
            {
                if (argv[i][j] == 'n')
                    opt->nflag = true;

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

char** getFileContent (char* path, size_t *length, size_t *bufferSize)
{
    char** lines = calloc(BUFFER_SIZE, sizeof(char*));

    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        lines[i] = calloc(BUFFER_SIZE, sizeof(char));
    }

    size_t nbLines = 0;
    size_t buffSize = BUFFER_SIZE;
    FILE *f;

    if (path[0] == '\0' || path[0] == '-')
        f = stdin;
    else
        f = fopen(path, "r");
    while(fgets(lines[nbLines], BUFFER_SIZE, f)) 
	{
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

    // reverse array
    for(size_t i = 0; i < nbLines/2; i++)
    {
        char* temp = lines[i];
        lines[i] = lines[nbLines-i-1];
        lines[nbLines-i-1] = temp;
    }

    *length = nbLines;
    *bufferSize = buffSize;
    return lines;
}

int singleFile(char* path, BuiltinFd *builtinFd, Options opt)
{
    size_t length;
    size_t bufferSize;
    if (opt.ind == -1)
    {
        char** lines = getFileContent(path, &length, &bufferSize);
        size_t i = 0;
        while(i < length)
        {
            fprintf(builtinFd->out, "%s", lines[i]);
            i++;
        }
        // freeeing lines
        for (size_t i = 0; i < bufferSize; i++)
        {
            free(lines[i]);
        }
        free(lines);
    }
    else
    {
        char** lines = getFileContent(path, &length, &bufferSize);

        if (opt.nflag)
        {
            // numbered
            size_t i = 0;
            while(i < length)
            {
                fprintf(builtinFd->out, "%lu %s", i, lines[i]);
                i++;
            }
            // freeeing lines
            for (size_t i = 0; i < bufferSize; i++)
            {
                free(lines[i]);
            }
            free(lines);
        }
        else
        {
            // not numbered
            size_t i = 0;
            while(i < length)
            {
                fprintf(builtinFd->out, "%s", lines[i]);
                i++;
            }
            // freeeing lines
            for (size_t i = 0; i < bufferSize; i++)
            {
                free(lines[i]);
            }
            free(lines);
        }
    }
    return 0;
}

int multipleFiles (char** argv, BuiltinFd *builtinFd, Options opt, size_t argc)
{
    size_t i = 0;
    if (opt.ind != -1)
	i = opt.ind;
    while (i < argc)
    {
        if (singleFile(argv[i], builtinFd, opt) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
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
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "tac: missing parameter\n");
        return -1;
    }
    else if (argc == 1)
    {
        if (singleFile(argv[0], builtinFd, opt) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }
    else // more than 1 argument
    {
        getOptions(argv, &opt, argc);
        if (multipleFiles(argv, builtinFd, opt, argc) == -1)
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
    terminal->in = stdin; //STDIN_FILENO;
    terminal->out = stdout; //(FILE *) STDOUT_FILENO;
    terminal->err = stderr; //(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "tac", terminal);
    int res = tac(argv,terminal);
    free(terminal);
    return res;
}
