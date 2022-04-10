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

bool onlyNumbers(char** lines, char*** notNumb, size_t length, size_t* count)
{
    bool res = true;
    size_t nb = 0;
    for (size_t i = 0; i < length; i++)
    {
        bool number = true;
        for(size_t j = 0; lines[i][j] != '\0'; j++)
        {
            if (isdigit(lines[i][j]))
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
            *notNumb[nb] = lines[i];
            nb++;
        }
    }
    *count = nb;

    return res;
}

int asciiSort(char** lines, size_t length)
{
    for (size_t i = 0; i < length; i++) // length -1? or length ?
    {
        for (size_t j = 0; j < length-i-1; j++)
        {
            if (lines[j][0] > lines[j+1][0])
            {
                char* temp = lines[j];
                lines[j] = lines[j+1];
                lines[j+1] = temp;
            }
        }
    }
    return 0;
}

size_t isMonth(char* test)
{
    // returns the number of the month
    char months[12][9] = {"january", "febuary", "march", "april", "may", "june",
        "july", "august", "september", "november", "december"};
    size_t l = strlen(test) + 1;
    char* n = calloc(l, sizeof(char));
    for (size_t j = 0; j < l; j++)
	n[j] = tolower(test[j]);

    n[l-1] = '\0';
    for (size_t i = 1; i < 13; i++)
    {
        if (strcmp(months[i-1], n) == 0)
            return i;
    }

    free(n);
    return 0;
}

bool onlyMonths(char** lines, char** notNumb, size_t length, size_t* count)
{
    bool res = true;
    size_t nb = 0;
    for (size_t i = 0; i < length; i++)
    {
	size_t month = isMonth(lines[i]);
        if (month > 0)
	{
	    continue;
	}
	else
        {
            notNumb[nb] = lines[i];
            nb++;
	    res = false;
        }
    }
    if (!res)
    	*count = nb;
    return res;
}

int monthsSort(char** lines, size_t length)
{
    for (size_t i = 0; i < length; i++) // length -1? or length ?
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

char* isSorted(char** lines, size_t length, char* path)
{
    char* line = calloc(BUFFER_SIZE, sizeof(char));
    for (size_t i = 0; i < length; i++)
    {
        if (lines[i][0] > lines[i+1][0])
        {
	    const char *s = "sort: ";
            strncpy(line, s, strlen(s));
	    strncpy(line + strlen(s), path, strlen(path));
	    line[strlen(s) + strlen(path)] = ':';
	    //printf("i is: %lu\n", i);
	    size_t count = 1;
	    size_t n = i + 2;
	    while (n > 9)
	    {
		n = n / 10;
		count++;
	    }
	    char str[count];
	    sprintf(str, "%zu", i + 2);
	    strncpy(line + strlen(s) + strlen(path) + 1, str, count);
	    const char *s2 = ": disorder: ";
	    strncpy(line + strlen(s) + strlen(path) + count + 1, s2, strlen(s2));
	    strncpy(line+strlen(s)+strlen(path)+count +1+strlen(s2), 
			    lines[i + 1], strlen(lines[i + 1]));
            break;
        }
    }
    return line;
}

size_t removeDuplicates(char** lines, size_t length)
{
    // check if this works
    if (length <= 1)
        return length;

    for (size_t i = 0; i < length; i++)
    {
	if (strcmp(lines[i],lines[i+1]) == 0)
	{
	    //upshift(lines, i+2, length);
            for (size_t j = i+2; j < length; j++)
	    {
		char* temp = lines[j-1];
		lines[j-1] = lines[j];
		lines[j] = temp;
	    }
	    i--;
	    length--;
        }
    }
    return length;
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

void printLines(FILE* path, char** lines, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf(path, "%s\n",lines[i]);
    }
    fflush(path);
}

int sortFile (char* src, char* dest, BuiltinFd *builtinFd, Options opt)
{
    size_t length;
    size_t count = 0;
    size_t bufferSize;
    char** lines = getFileContent(src, &length, &bufferSize);
    if (opt.ind == 0)
    {
        // no options
        if (asciiSort(lines, length) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
	printLines(builtinFd->out, lines, length);
	return 0;
    }
    else
    {
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
            fprintf(builtinFd->out, "%s\n", mistake);

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
	    //printf("m flag is on\n");
            if (onlyMonths(lines, diff, length, &count))
            {
		//printf("only months\n");
                monthsSort(lines, length);
                count = 0;
            }
            else
            {
                // first sort alphabetically and then monthly
                asciiSort(diff, count);
                monthsSort(lines, length-count);
            }
        }
	else
	{
	    asciiSort(lines, length);
	}

        // can be combined
        if (opt.uflag)
        {
            if (count != 0)
            {
                size_t temp = count;
                count = removeDuplicates(diff, count);
                length = removeDuplicates(lines, length-temp) + count;
		asciiSort(lines, length);
		asciiSort(diff, count);
            }
            else
            {
		length = removeDuplicates(lines, length);
		asciiSort(lines, length);
	    }
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

	// PRINTING
        if (opt.oflag) 
        {
	   // printf("opt flag is true\n");
            if (count != 0)
            {
		FILE* f = fopen(dest, "w+");
                printLines(f, diff, count);
                printLines(f, lines, length-count);
		fclose(f);
            }
            else
	    {
		FILE* f = fopen(dest, "w+");
                printLines(f, lines, length);
		fclose(f);
	    }
        }
        else 
        {
	    //printf("count = %lu\n", count);
            if (count != 0)
            {
                printLines(builtinFd->out, diff, count);
                printLines(builtinFd->out, lines, length-count);
            }
            else
                printLines(builtinFd->out, lines, length);
        }

        // freeing diff
        /*for (size_t i = 0; i < length; i++)
        {
            free(diff[i]);
        }
        free(diff);*/
    }

    // freeeing lines
    for (size_t i = 0; i < bufferSize; i++)
    {
        free(lines[i]);
    }
    free(lines);
    return 0;
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
    /*argv[0] = argv[1];
    argv[1] = argv[2];
    argv[2] = NULL;*/

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "sort: missing parameter\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        getOptions(argv, &opt, argc);
	//for (size_t i = 0; i < argc; i++)
	//	printf("argv[%lu] = %s\n", i, argv[i]);
        char* dest;
        char* src;
        if (opt.ind+1 >= (ssize_t)argc)
        {
            dest = NULL;
            src = argv[opt.ind];
        }
        else
        {
            src = argv[opt.ind+1];
            dest = argv[opt.ind];
        }
	//printf("src = '%s', dest = '%s'\n", src, dest);
        if (sortFile(src, dest, builtinFd, opt) == -1)
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
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    sort(argv,terminal);
    free(terminal);
}
