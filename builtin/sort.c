#include "sort.h"


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
    length = nbLines;
    bufferSize = buffSize;
    return lines;
}

bool onlyNumbers(char** lines, char*** notNumb, size_t length, size_t* count)
{
    bool res = true;
    size_t nb = 0;
    for (size_t i = 0; i < length; i++)
    {
        bool number = true;
        for(size_t j = 0; lines[j] != '\0'; j++)
        {
            if (isNumber(lines[i][j]))
                continue;
            else
            {
                number = false;
                res = false;
                break;
            }
        }
        if (!number)
        {
            notNumb[nb] = lines[i];
            nb++;
        }
    }

    return res;
}

int asciiSort(char** lines, size_t length)
{
    for (size_t i = 0; i < length-1; i++) // length -1? or length ?
    {
        for (size_t j = 0; j < length-i-1; j++)
        {
            if (lines[j] > lines[j+1])
            {
                char* temp = lines[j];
                lines[j] = lines[j+1];
                lines[j+1] = temp;
            }
        }
    }
    return 0;
}

int isMonth(char* test)
{
    // returns the number of the month
    char months[12][9] = {"january", "febuary", "march", "april", "may", "june",
        "july", "august", "september", "november", "december"};

    for (int i = 0; i < 12; i++)
    {
        if (months[i] == tolower((unsigned char) test))
            return i;
    }
    return -1;
}

bool onlyMonths(char** lines, char*** notNumb, size_t length, size_t* count)
{
    bool res = true;
    size_t nb = 0;
    for (size_t i = 0; i < length; i++)
    {
        bool number = true;
        for(size_t j = 0; lines[j] != '\0'; j++)
        {
            if (isMonth(lines[i][j]) != -1)
                continue;
            else
            {
                number = false;
                res = false;
                break;
            }
        }
        if (!number)
        {
            notNumb[nb] = lines[i];
            nb++;
        }
    }

    return res;
}

int monthsSort(char** lines, size_t length)
{
    for (size_t i = 0; i < length-1; i++) // length -1? or length ?
    {
        for (size_t j = 0; j < length-i-1; j++)
        {
            if (isMonth(lines[j]) > isMonth(lines[j+1]))
            {
                char* temp = lines[j];
                lines[j] = lines[j+1];
                lines[j+1] = temp;
            }
        }
    }
    return 0;
}

char* isSorted (char** lines, size_t length, char* path)
{
    char* line = calloc(BUFFER_SIZE, sizeof(char));
    for (size_t i = 0; i < length; i++)
    {
        if (lines[i] > lines[i+1])
        {
            strcpy(line, "sort: ");
            strcat(line, path);
            strcat(line, ":");
            strcat(line, i);
            strcat(line, ": disorder: ");
            strcat(line, lines[i]);
            break;
        }
    }
    return line;
}

void removeDuplicates(char** lines, size_t length)
{
    // TODO: check if this works
    char* last = lines[0];
    for (size_t i = 1; i < length; i++)
    {
        if (last == lines[i])
        {
            for (size_t j = i; j < length; j++)
            {
                lines[j] = lines[j+1];
            }
            length -= 1;
        }
        else
            last = lines[i];
    }
}

void reverseArray(char** lines, size_t length)
{
    for(size_t i = 0; i < length/2; i++)
    {
        char* temp = lines[i];
        lines[i] = lines[length-i-1];
        lines[length-i-1] = temp;
    }
}

void printLines(char* path, char** lines, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf(path, lines[i]);
    }
}

int sortFile (char* src, char* dest, BuiltinFd *builtinFd, Options opt, 
    size_t argc)
{
    size_t length;
    size_t bufferSize;
    char** lines = getFileContent(src, &length, &bufferSize);
    if (opt.ind == -1)
    {
        // no options
        if (asciiSort(lines, length) == -1)
            return -1;
    }
    else
    {
        size_t count;
        char** diff = calloc(length, sizeof(char*));

        for (size_t i = 0; i < length; i++)
        {
            diff[i] = calloc(length, sizeof(char));
        }
        if (opt.nflag)
        {
            if (onlyNumbers(lines, &diff, length, &count))
            {
                asciiSort(lines, length);

                for (size_t i = 0; i < length; i++)
                {
                    free(diff[i]);
                }
                free(diff);
                count = 0;
            }
            else
            {
                // first sort alphabetically and then numerically
                asciiSort(diff, count);
                asciiSort(lines, length-count);
            }
        }
        else if (opt.cflag)
        {

            char* mistake = isSorted(lines, length, src);
            fprintf(builtinFd->out, mistake);
            free(mistake);
            // freeing diff
            for (size_t i = 0; i < length; i++)
            {
                free(diff[i]);
            }
            free(diff);
    
            // freeeing lines
            for (size_t i = 0; i < bufferSize; i++)
            {
                free(lines[i]);
            }
            free(lines);

            return 0;
        }
        else if (opt.Mflag) 
        {
            if (onlyMonths(lines, &diff, length, &count))
            {
                monthsSort(lines, length);

                for (size_t i = 0; i < length; i++)
                {
                    free(diff[i]);
                }
                free(diff);
                count = 0;
            }
            else
            {
                // first sort alphabetically and then monthly
                asciiSort(diff, count);
                monthsSort(lines, length-count);
            }
        }

        // can be combined
        if (opt.uflag)
        {
            if (count != 0)
            {
                removeDuplicates(diff, count);
                removeDuplicates(lines, length-count);
            }
            else
                removeDuplicates(lines, length);
        }
        if (opt.rflag) 
        {
            if (count != 0)
            {
                reverseArray(diff, count);
                reverseArray(lines, length-count);
            }
            else
                reverseArray(lines, length);
        }
        if (opt.oflag) 
        {
            if (count != 0)
            {
                printLines(dest, diff, count);
                printLines(dest, lines, length-count);
            }
            else
                printLines(dest, lines, length);
        }
        else 
        {
            if (count != 0)
            {
                printLines(builtinFd->out, diff, count);
                printLines(builtinFd->out, lines, length-count);
            }
            else
                printLines(builtinFd->out, lines, length);
        }

        // freeing diff
        for (size_t i = 0; i < length; i++)
        {
            free(diff[i]);
        }
        free(diff);
    }

    // freeeing lines
    for (size_t i = 0; i < bufferSize; i++)
    {
        free(lines[i]);
    }
    free(lines);

}

/**
** @brief               sort main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int sort(char** argv, BuiltinFd *builtinFd)
{
    Options opt;
    opt.oflag = false;
    opt.rflag = false;
    opt.nflag = false;
    opt.cflag = false;
    opt.uflag = false;
    opt.Mflag = false;
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "sort: missing parameter\n");
        return -1;
    }
    else
    {
        getOptions(argv, &opt, argc);
        char* dest;
        char* src;
        if (opt.ind+1 >= argc)
        {
            dest = NULL;
            src = argv[opt.ind];
        }
        else
        {
            src = argv[opt.ind+1];
            dest = argv[opt.ind];
        }
        if (sortFile(src, dest, builtinFd, opt, argc) == -1)
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
    sort(argv,terminal);
    free(terminal);
}