#include "procStatus.h"
#define _GNU_SOURCE

/**
** @brief               Get the process status from the process ID.
** @param pid           Process ID.
** @param builtinFd     Files.
** @return              Returns the status of the process
*/
char* get_process_status_by_pid(const int pid, BuiltinFd *builtinFd)
{
    builtinFd = builtinFd;
    char* status = (char*)calloc(1024,sizeof(char));

    if(status){
        sprintf(status, "/proc/%d/status",pid);
        FILE* f = fopen(status,"r");
        if(f){
            size_t size;
            size = fread(status, sizeof(char), 1024, f);
            if(size>0){
                if('\n'==status[size-1])
                    status[size-1]='\0';
            }
            fclose(f);
        }
        //fprintf(builtinFd->out, "%s\n", name);
        return status;
    }

    fprintf(builtinFd->err, "ProcStatus: Not enough memory to allocate.\n");

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
    AppendToHistory(argv, "procstatus", terminal);

    if (argc){}
    if (argv[0] == NULL)
    {
        fprintf(terminal->out, "TEST\n");
        pid_t pid = getpid();
        char *status = get_process_status_by_pid(pid, terminal);
        fprintf(terminal->out, "%s\n", status);
    }
    else //if (argv[0])
    {
        pid_t pid = atoi(argv[0]);
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "/proc/%u/status", pid);
        char *status = get_process_status_by_pid(pid, terminal);

        //struct stat sts;
        /*if (stat("/proc/<1>/", &sts) == -1 && errno == ENOENT)
        {
            fprintf(terminal->err, "ProcStatus: Invalid process ID.\n");
            //return 1;
        }*/

        if (strcmp(buffer, status) == 0)
        {
            fprintf(terminal->err, "ProcStatus: Process doesn't exist!\n");
            exit(EXIT_FAILURE);
            return -1;
        }

        fprintf(terminal->out, "%s\n", status);
    }
    /*else
    {
        fprintf(terminal->err, "ProcStatus: Invalid number of arguments.\n");
        return 1;
    }*/

    return 0;
}
