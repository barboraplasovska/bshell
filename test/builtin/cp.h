#ifndef CP_H
#define CP_H

#include "builtin.h"

/**
** @brief               cp main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cp(char** argv, int argc, BuiltinFd *builtinFd);

#endif
