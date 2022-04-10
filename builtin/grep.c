#define _GNU_SOURCE
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
	    i = 0;
        for (; i < argc; i++)
        {
            if (expectPattern)
            {
                opt->patterns[j] = argv[i];
                j++;
                opt->ecount++;

		        if (opt->ecount >= *bufferSize)
	            {
                    *bufferSize *= 2;
                    opt->patterns = 
                    realloc(opt->patterns, *bufferSize * sizeof(char*));
                    for (size_t k = 0; k < *bufferSize; k++)
                        opt->patterns[k] =
                    realloc(opt->patterns[k], *bufferSize*sizeof(char));
                }
            }
            else if (strcmp(argv[i], "-e") == 0)
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
        opt->n = (size_t)argv[opt->ind];
        opt->ind++;
    }
}

char** searchFile(char* pattern, char* path, size_t *length, Options opt)
{
    char** lines = NULL;

    size_t nbLines = 0;
    size_t totalNumberOfLines = 0;

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        //error opening file
    }

    while((nread = getline(&line, &len, fp)) != -1) //fgets(line, BUFFER_SIZE, fp)
    {
        totalNumberOfLines++;
	    //printf("%s", line);
        //fwrite(line, nread, 1, stdout);
        if (opt.iflag ? strcasestr(line, pattern) : strstr(line, pattern))
        {
            nbLines++;
            lines = realloc(lines, nbLines * sizeof(char*));
            if (!lines)
            {
                //error
            }

            if (opt.nflag)
            {
                size_t count = 1;
                size_t n = totalNumberOfLines;
                while (n > 9)
                {
                    n = n / 10;
                    count++;
                }
                char str[count];
                sprintf(str, "%zu", totalNumberOfLines);
                lines[nbLines - 1] = malloc(len + count + 1);
                strncpy(&(lines[nbLines - 1][0]), str, count);
                lines[nbLines - 1][count] = ':';
                strncpy(&lines[nbLines - 1][count + 1], line, len);
            }
            else
            {
                lines[nbLines - 1] = malloc(len);
                char *lineCopy = malloc(len);
                strncpy(lineCopy, line, len);
                lines[nbLines - 1] = lineCopy;
            }
        }
    }
    //printf("exit...\n");
    free(line);
    *length = nbLines;
    return lines;
}

void printLines(char** lines, size_t length, FILE* file)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf(file, "%s", lines[i]);
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

        /*printf("the index is: %ld\n", opt.ind);
        for (size_t i = 0; i < argc; i++)
        {
            printf("index: %li: %s\n", i, argv[i]);
        }*/
        
        lines = searchFile(argv[opt.ind], argv[opt.ind + 1], &length, opt);

        /*
        if (opt.ind == 0) //no options
        {
	        lines = searchFile(argv[0], argv[1], &length, opt);
        }
        else if(opt.nflag)
        {
            lines = searchFile(argv[1], argv[2], &length, opt);
        }
        else if (opt.iflag)
        {
            for (size_t i = 0; i < length; i++)
            {
                for (size_t j = 0; lines[i][j] != '\0'; ++j) 
                    lines[i][j] = tolower((unsigned char) lines[i][j]);
            }
        }*/

        printLines(lines, length, builtinFd->out);

        //uncomment    printLines(lines, length, builtinFd->out);
        
        // freeing lines
        for (size_t i = 0; i < length; i++)
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
    AppendToHistory(argv, "grep", terminal);
    grep(argv,terminal);
    free(terminal);
}
