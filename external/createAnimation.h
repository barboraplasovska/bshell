#ifndef CA_H
#define CA_H
#define MAX_LIMIT 256

#include <string.h>

#include "../builtin/builtin.h"

/**
** @brief               Create an animation.
** @param name          Animation name.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int createAnimation(char* name, BuiltinFd *builtinFd);

#endif