#ifndef CD_H
#define CD_H

#include "../builtin/builtin.h"

/**
** @brief               goes through the history of the commands and allows to call them
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
void history(BuiltinFd *builtinFd);

#endif