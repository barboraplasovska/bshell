#ifndef TAC_H
#define TAC_H

#include "builtin.h"

/**
** @brief               Builtin tac options.
** @param nflag         Option -n is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  nflag; // numbered
  size_t                ind;
} Options;


/**
** @brief               tac main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int tac(char** argv, BuiltinFd *builtinFd);

#endif
