#ifndef TOUCH_H
#define TOUCH_H

#include "builtin.h"

/**
** @brief               touch main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int touch(char** argv, BuiltinFd *builtinFd);

#endif