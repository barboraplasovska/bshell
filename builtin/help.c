#include "help.h"

int help(BuiltinFd *builtinFd)
{
    FILE* f;
    char buffer[BUFFER_SIZE*3]; //characer buffer to store the bytes
    f = fopen("./builtin/help.txt", "r+");
    if(f == NULL)
    {
        //fprintf(builtinFd->err, "cat: error: cannot open file");
        exit(EXIT_FAILURE);
        return -1;
    }
    while(fgets(buffer, BUFFER_SIZE, f))
    {
        fprintf(builtinFd->out, "%s", buffer);
    }
    fclose(f);

    return 0;
}

int main()
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    int res = help(terminal);
    free(terminal);
    return res;

}
