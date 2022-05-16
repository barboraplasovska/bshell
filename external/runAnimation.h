#ifndef RA_H
#define RA_H
#define _GNU_SOURCE
#define MAX_LIMIT 256

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"


/**
** @brief               Builtin echo options.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  lflag;
  ssize_t                ind;
} Options;

/**
** @brief               Run The Animation using animationName
** @param name          Name of the animation.
** @param builtinFd     Files.
** @return              return 0 on success
*/
int runAnimation(char *name, bool lflag, BuiltinFd *builtinFd);

#endif
