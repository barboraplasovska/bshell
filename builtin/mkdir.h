#ifndef MKDIR_H
#define MKDIR_H

#include "builtin.h"

/**
** @brief               mkdir main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int mkdir(char** argv, BuiltinFd *builtinFd);

#endif