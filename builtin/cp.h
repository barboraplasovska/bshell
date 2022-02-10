#ifndef CAT_H
#define CAT_H

#include "builtin.h"

/**
** @brief               cp main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cp(char** argv, BuiltinFd *builtinFd);

#endif