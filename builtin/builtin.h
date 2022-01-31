#ifndef BUILTIN_H
#define BUILTIN_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

/**
** @brief               Builtin files.
** @param in            Input file.
** @param out           Output file.
** @param err           Error file.
*/
typedef struct          builtinFd
{
  FILE                  *in;
  FILE                  *out;
  FILE                  *err;
} BuiltinFd;

#endif