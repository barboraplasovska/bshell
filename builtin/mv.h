#ifndef MV_H
#define MV_H

#include "builtin.h"


/**
** @brief               Move main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int mv(char** argv, BuiltinFd *builtinFd);

#endif
