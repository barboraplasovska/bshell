#ifndef HEAD_H
#define HEAD_H

#include "builtin.h"

// tail can be used with multiple files !!!
// FIRST it says "tab filename"
// then the last 10 lines of file

/**
** @brief               Builtin tac options.
** @param nflag         Option -n is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                   nflag; // prints last n lines "tail -n 3 file.txt"
  size_t                 nb;
  bool                   qflag; // data from each file is not preceded by filename
  bool                   vflag; // data of the specified file is ALWAYS preceded by filename
                                // ==> state.txt <==
  ssize_t                ind;
} Options;

/**
** @brief               head main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int head(char** argv, BuiltinFd *builtinFd);

#endif