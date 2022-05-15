#ifndef RA_H
#define RA_H
#define MAX_LIMIT 256

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

#include "../builtin/builtin.h"

/**
** @brief               Remove an animation.
** @param name          Animation name.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int removeAnimation(char* name, BuiltinFd *builtinFd);

#endif