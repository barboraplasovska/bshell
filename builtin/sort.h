#ifndef SORT_H
#define SORT_H

#include "builtin.h"

/**
** @brief               Builtin echo options.
** @param nflag         Option -s is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  oflag; // output to a new file
  bool                  rflag; // Sorting In Reverse Order
  bool                  nflag; //  sort a file numerically
  bool                  cflag; //  f the file given is already sorted or not ( returns nothing if sorted, else "sort: cars.txt:3: disorder: BMW")
  bool                  uflag; // To sort and remove duplicates
  bool                  Mflag; // To sort by month 
  size_t                ind;
} Options;


/**
** @brief               tac main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int sort(char** argv, BuiltinFd *builtinFd);

#endif