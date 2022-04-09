#ifndef LS_H
#define LS_H

#include "builtin.h"


/**
** @brief               Ls main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int ls(char** argv, BuiltinFd *builtinFd);

#endif