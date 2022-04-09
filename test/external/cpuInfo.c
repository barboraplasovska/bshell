#include "cpuInfo.h"
#define _GNU_SOURCE

/**
** @brief               Get the process status from the process ID.
** @param pid           Process ID.
** @param builtinFd     Files.
** @return              Returns the status of the process
*/
char* GetCPUInfo(BuiltinFd *builtinFd)
{
    char* status = (char*)calloc(16384,sizeof(char));

    if(status){
        sprintf(status, "/proc/cpuinfo");
        FILE* f = fopen(status,"r");
        if(f){
            size_t size;
            size = fread(status, sizeof(char), 16384, f);
            if(size>0){
                if('\n'==status[size-1])
                    status[size-1]='\0';
            }
            fclose(f);
        }
        //fprintf(builtinFd->out, "%s\n", name);
        return status;
    }

    fprintf(builtinFd->err, "CPUInfo: Not enough memory to allocate.\n");
    return status;
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

    if (argc){}
    if (argv[1] == NULL)
    {
        char* info = GetCPUInfo(terminal);
        fprintf(terminal->out, "%s\n", info);
    }
    else
    {
        fprintf(terminal->err, "CPUInfo: Invalid number of arguments.\n");
        return 1;
    }

    return 0;
}
