#ifndef AFC_H
#define AFC_H
#define MAX_LIMIT 256

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

#include "../builtin/builtin.h"

/**
** @brief               Counts number of framces in an animation
** @param name          Animation name.
** @param builtinFd     Files.
** @return              return 0 on success
*/
int countAnimationFrames(char* name, BuiltinFd *builtinFd);

#endif