#include "runAnimation.h"
//#define  _POSIX_C_SOURCE

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
            while (argv[i][j] == 'l')
            {
                if (argv[i][j] == 'l')
                    opt->lflag = true;

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

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void clearScreen()
{
    system("clear");
}

void ExecuteFrame(char* str, BuiltinFd *builtinFd)
{
    fprintf(builtinFd->out, "%s", str);
    fflush(builtinFd->out);
    msleep(150);
    clearScreen();
    fflush(builtinFd->out);
    msleep(20);
}

char* itoa(int value, char* result, int base) \
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

/**
** @brief               Run The Animation using animationName
** @param name Name of the animation.
** @param builtinFd     Files.
** @return              return 0 on success
*/
int runAnimation(char *name, BuiltinFd *builtinFd)
{

    struct stat st = {0};

    size_t pathLength = 11 + strlen(name) + 1;
    char *path = malloc(pathLength);
    strncpy(path, "Animations/", 12);

    for (size_t i = 11; i < pathLength - 1; i++)
    {
        path[i] = name[i - 11];
    }
    path[pathLength - 1] = '\0';

    if (stat(path, &st) != 0)
    {
        fprintf(builtinFd->err, "runanimation: %s doesn't exist.\n", name);
        return -1;
    }

    size_t pathLengthInitial = pathLength;

    size_t frameNumber = 1;

    clearScreen();

    do
    {
        pathLength = pathLengthInitial;
        char snum[5];
        itoa(frameNumber, snum, 10);
        size_t numDigits = strlen(snum);
        pathLength += numDigits;
        path = realloc(path, pathLength);
        path[pathLength - numDigits - 1] = '/';
        path[pathLength] = '\0';

        for(size_t i = pathLength - numDigits; i < pathLength; i++)
        {
            path[i] = snum[i - pathLength + numDigits];
        }

        if (stat(path, &st) != 0)
            break;
        
        //char content[MAX_LIMIT];
        FILE *fp;
        fp = fopen(path , "r");
        if(fp == NULL) 
        {
            fprintf(builtinFd->err, "runAnimation: Error opening file\n");
            return -1;
        }

        char * line = NULL;
        size_t totalLength = 0;
        size_t len = 0;
        ssize_t read;

        char *total = malloc(totalLength);

        while ((read = getline(&line, &len, fp)) != -1) 
        {
            totalLength += read;
            total = realloc(total, totalLength);
            for (size_t i = totalLength - read; i < totalLength; i++)
            {
                total[i] = line[i - totalLength + read];
            }
        }
        total[totalLength] = '\0';
        //printf("our tota length is: %zu", totalLength);
        //printf("%s", total);
        ExecuteFrame(total, builtinFd);

        frameNumber++;

        fclose(fp);
    } while (1);

    fprintf(builtinFd->out, "\n");

    return 0;
}

int main(int argc, char **argv) //TODO: get options for l to loop runAnimation
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "runanimation", terminal);

    if (argc){}
    if (argv[1] != NULL)
    {
        fprintf(terminal->err, "runanimation: This function only takes one argument.\n");
        return -1;
    }

    Options opt;
    opt.ind = -1;
    opt.lflag = false;

    getOptions(argv, &opt, argc);

    do
    {
        runAnimation(argv[0], terminal);
    } 
    while (opt.lflag);
    
    return 0;
}
