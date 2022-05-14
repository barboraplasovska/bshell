#ifndef UNALIAS_H
#define UNALIAS_H

#include "builtin.h"

/**
** @brief               unalias main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int unalias(char** argv, BuiltinFd *builtinFd);


#endif