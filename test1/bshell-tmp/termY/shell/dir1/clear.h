#ifndef CLEAR_H
#define CLEAR_H

#include "builtin.h"

/**
** @brief               Clear main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int clear(char** argv, BuiltinFd *builtinFd);

#endif