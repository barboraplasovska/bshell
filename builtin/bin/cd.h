#ifndef CD_H
#define CD_H

#include "builtin.h"

/**
** @brief               cd main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cd(char** argv, BuiltinFd *builtinFd);

#endif