#ifndef HELP_H
#define HELP_H

#include "builtin.h"

/**
** @brief               help main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int help(char** argv, BuiltinFd *builtinFd);

#endif