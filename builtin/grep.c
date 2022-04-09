#include "grep.h"

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t *bufferSize, size_t argc)
{
    size_t i, j = 0;//1 
    for (i = 0; i < argc; i++) //1
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'c' || argv[i][j] == 'h' || argv[i][j] == 'i' 
            || argv[i][j] == 'l' || argv[i][j] == 'n' || argv[i][j] == 'v'
            || argv[i][j] == 'e' || argv[i][j] == 'f' || argv[i][j] == 'w'
            || argv[i][j] == 'o' || argv[i][j] == 'A' || argv[i][j] == 'B'
            || argv[i][j] == 'C' )
            {
                if (argv[i][j] == 'c')
                    opt->cflag = true;
                if (argv[i][j] == 'h')
                    opt->hflag = true;
                if (argv[i][j] == 'i')
                    opt->iflag = true;
                if (argv[i][j] == 'l')
                    opt->lflag = true;
                if (argv[i][j] == 'n')
                    opt->nflag = true;
                if (argv[i][j] == 'v')
                    opt->vflag = true;
                if (argv[i][j] == 'e')
                {
                    opt->eflag = true;
                    opt->ecount = 1;
                }

                if (argv[i][j] == 'f')
                    opt->fflag = true;
                
                if (argv[i][j] == 'w')
                    opt->wflag = true;
                if (argv[i][j] == 'o')
                    opt->oflag = true;
                if (argv[i][j] == 'A')
                    opt->Aflag = true;
                if (argv[i][j] == 'B')
                    opt->Bflag = true;
                if (argv[i][j] == 'C')
                    opt->Cflag = true;

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
    
    if (opt->eflag)
    {
        bool expectPattern = false;
        size_t j = 0;
        for (; i < argc; i++)
        {
            if (expectPattern)
            {
                opt->patterns[j] = argv[i];
                j++;
                opt->ecount++;
            }
            else if (argv[i] == '-e')
            {
                expectPattern = true;
            }
            else
                break;
        }
        if (i < argc)
            opt->ind = i;
    }

    if (opt->fflag)
    {
        opt->filename = argv[opt->ind];
        opt->ind++;
    }

    if (opt->Aflag || opt->Bflag || opt->Cflag)
    {
        opt->n = argv[opt->ind];
        opt->ind++;
    }
}

char** searchFile(char* path, size_t *length, size_t *bufferSize, Options opt)
{
    char** lines = calloc(BUFFER_SIZE, sizeof(char*));

    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        lines[i] = calloc(BUFFER_SIZE, sizeof(char));
    }

    size_t nbLines = 0;
    size_t buffSize = BUFFER_SIZE;

    FILE *fp;
    char line[BUFFER_SIZE];
    fp = fopen(path, "r");
    char delim[] = " ";
 
    while(fgets(line, BUFFER_SIZE, fp)) 
	{
        char* word = strok(line, delim);
        while(word != NULL)
        {
            for (size_t i = 0; i < opt.ecount; i++)
            {
                if (word == opt.patterns[i])
                {
                    if (opt.nflag)
                    {
                        char* str = "";
                        strcat(str, i);
                        strcat(str, ":");
                        strcat(str, line);
                        lines[nbLines] = str;
                    }
                    else
                        lines[nbLines] = line;
                    nbLines++;
                    if (nbLines >= buffSize)
                    {
                        buffSize *= 2;
                        lines = realloc(lines, buffSize * sizeof(char*));
                        for (size_t i = 0; i < buffSize; i++)
                        {
                            lines[i] = realloc(lines[i], 
                                buffSize * sizeof(char));
                        }
                    }
                    break;
                }
            }
            word = strtok(NULL, delim);
        }
    }
    length = nbLines;
    bufferSize = buffSize;
    return lines;
}

void printLines(char** lines, size_t length, FILE* file)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf(file, "%s\n", lines[i]);
    }
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
    opt.ecount = 0; 
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
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        getOptions(argv, &opt, &bufferSize, argc);
        char** lines;
        size_t length;
        size_t bufferSize;

        if (opt.ind == -1) // no options
        {
            lines = searchFile(argv, &length, &bufferSize, opt);
        }
        if (opt.iflag)
        {
            for (size_t i = 0; i < length; i++)
            {
                for (size_t j = 0; lines[i][j] != '\0'; ++j) 
                    lines[i][j] = tolower((unsigned char) lines[i][j]);
            }
        }

        printLines(lines, length, builtinFd->out);
        
        // freeing lines
        for (size_t i = 0; i < bufferSize; i++)
            free(lines[i]);
        free(lines);
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