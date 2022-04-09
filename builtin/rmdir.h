#ifndef RMDIR_H
#define RMDIR_H

#include "builtin.h"

/**
** @brief               rmdir main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int rmdir(char** argv, BuiltinFd *builtinFd);

#endif