#ifndef ALIAS_H
#define ALIAS_H

#include "builtin.h"

/**
** @brief               Builtin echo options.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  pflag;
  ssize_t                ind;
} Options;

/**
** @brief               cat main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int alias(char** argv, BuiltinFd *builtinFd);

#endif