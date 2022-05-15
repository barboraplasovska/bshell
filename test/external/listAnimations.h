#ifndef LA_H
#define LA_H

#include <string.h>

#include <dirent.h>
#include <errno.h>

#include "../builtin/builtin.h"

/**
** @brief               list animations.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int listAnimations(BuiltinFd *builtinFd);

#endif