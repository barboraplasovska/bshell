#include "help.h"

int help(char** argv, BuiltinFd *builtinFd)
{
    fprintf(builtinFd->out, "Terminal powered by NaN (A1 S4)\n\n");
    int f;
    int count;
    char buffer[BUFFER_SIZE*3]; 
    f = open("../README", O_RDONLY);
    while((count = read(f, buffer, sizeof(buffer))) > 0)
    {
        fprintf(builtinFd->out, "%s", buffer);
    }

    return 0;
}