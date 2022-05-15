#ifndef CA_H
#define CA_H

#include <string.h>

#include <dirent.h>
#include <errno.h>

#include "../builtin/builtin.h"

/**
** @brief               clear animations.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int clearAnimations(BuiltinFd *builtinFd);

#endif