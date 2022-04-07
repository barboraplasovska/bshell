#ifndef GREP_H
#define GREP_H

#include "builtin.h"


typedef struct          Options
{
  bool                  cflag; // prints only a count of the lines that match 
                               // a pattern
  bool                  hflag; // Displays the matched lines, but do not display
                               // the filenames.
  bool                  iflag; // Ignores lower/uppercase for matching
  bool                  lflag; // Displays list of a filenames only.
  bool                  nflag; // Display the matched lines and their line 
                               // numbers.
  bool                  vflag; // This prints out all the lines that do not 
                               // matches the pattern
  bool                  eflag; // Specifies expression with this option. 
                               // Can be used multiple times.
  size_t                ecount; // how many times has it been used
  char**                patterns; // patterns
  bool                  fflag; // -f file : Takes patterns from file, one per 
                               // line.
  char*                 filename; // filename
  bool                  wflag; // -w : Match whole word
  bool                  oflag; // -o : Print only the matched parts of a 
                               // matching line, with each such part on a 
                               // separate output line.
  bool                  Aflag; // -A n : Prints searched line and nlines after 
                               // the result.
  bool                  Bflag; // -B n : Prints searched line and nlines before 
                               // the result.
  bool                  Cflag; // -C n : Prints searched line and nlines before 
                               // and after the result.
  size_t                n;

  size_t                ind;
} Options;

/**
** @brief               grep main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int grep(char** argv, BuiltinFd *builtinFd);

#endif