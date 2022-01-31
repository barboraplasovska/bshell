#include "clear.h"

/**
** @brief               Clear main function.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int clear(BuiltinFd *builtinFd)
{
    fprintf(builtinFd->out, "'[H'[J");
    fflush(NULL);
    return 0;
}