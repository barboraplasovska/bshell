#include "procList.h"
#include <dirent.h>
#define _GNU_SOURCE

/**
** @brief               Lists all the process ids
** @param builtinFd     Files.
** @return              Returns the char double pointer of all the process ids running
*/

int isNumber(char s[])
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
            return 0;
    }
    return 1;
}

void string_copy(char* from, char* to) {

    while ((*to++ = *from++) != '\0')
        ;
}

int count_recur (int n) {
    int count = 1;
    while (n > 9)
    {
        n = n / 10;
        count = count + 1;
    }
    return count;
}

char** listProc(BuiltinFd *builtinFd)
{
    struct dirent* dp;
    const char* path = "/proc/";
    DIR* dir = opendir(path);

    if (!dir)
    {
        fprintf(builtinFd->err, "procList: Unable to open directory.\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    int numOfEls = 0;

    while ((dp = readdir(dir)) != NULL)
    {
        if (isNumber(dp->d_name))
            numOfEls += 1;
    }
    char** processes = (char**)malloc(numOfEls * sizeof(char*));
    int numOfDigitsInEl = count_recur(numOfEls);
    char numOfElsCharP[numOfDigitsInEl + sizeof(char)];
    processes[0] = (char*)malloc(numOfDigitsInEl * sizeof(char));
    sprintf(numOfElsCharP, "%d", numOfEls);
    string_copy(numOfElsCharP, processes[0]);
    int count = 1;

    dir = opendir(path);
    while ((dp = readdir(dir)) != NULL)
    {
        if (isNumber(dp->d_name))
        {
            int length = strlen(dp->d_name);
            processes[count] = (char*)malloc(length * sizeof(char));
            string_copy(dp->d_name, processes[count]);
            count++;
        }
    }
    closedir(dir);
    return processes;
}

int main(int argc, char **argv)
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "proclist", terminal);

    if (argc){}
    if (argv[0] == NULL)
    {
        char **processes = listProc(terminal);

        processes=processes;
        char *countC = processes[0];
        int count = atoi(countC);

        for (int i = 0; i < count; i++)
        {
            fprintf(terminal->err ,"Process_ID: %s\n", processes[i]);
        }
    }
    else
    {
        fprintf(terminal->err, "ProcList: This function doesn't take any arguments!\n");
        return 1;
    }

    return 0;
}
