#ifndef TAC_H
#define TAC_H

#include "builtin.h"

/**
** @brief               Builtin echo options.
** @param nflag         Option -s is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  sflag; // separator
  bool                  nflag; // numbered
  bool                  bflag; // seperator added before
  char                  separator;
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
