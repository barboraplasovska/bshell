#ifndef CAT_H
#define CAT_H

#include "builtin.h"

/**
** @brief               Builtin echo options.
** @param nflag         Option -n is activated.
** @param eflag         Option -e is activated.
** @param Eflag         Option -E is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  Aflag;
  bool                  Eflag;
  bool                  sflag;
  bool                  nflag;
  size_t                ind;
} Options;

typedef struct          Operators
{
  bool                  sm; // >
  bool                  doublesm; // >>
  bool                  star; // *.txt
  size_t                ind;
} Operators;

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc);


/**
** @brief               cat main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int cat(char** argv, BuiltinFd *builtinFd);

#endif
