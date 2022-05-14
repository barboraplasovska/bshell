#ifndef RM_H
#define RM_H

#include "builtin.h"

/**
** @brief               Builtin echo options.
** @param nflag         Option -i is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  iflag; // interactive (asks if sure to delete)
  bool                  fflag; // force delete
  bool                  rflag; // or -R recursive delet (for dir)
  ssize_t                ind;
} Options;


/**
** @brief               rm main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int rm(char** argv, BuiltinFd *builtinFd);

#endif
