#ifndef BUILTIN_H
#define BUILTIN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h> 
#include <fcntl.h>
//#include <iostream.h>
#include <sys/stat.h>
#include <errno.h>
//#include <sys/types.h>

#define BUFFER_SIZE 1024

/**
** @brief               Builtin files.
** @param in            Input file.
** @param out           Output file.
** @param err           Error file.
** @param inOut         Input file number.
** @param outOut        Output file number.
** @param errOut        Error file number.
*/
typedef struct          builtinFd
{
  FILE                  *in;
  FILE                  *out;
  FILE                  *err;
  int                   inNo;
  int                   outNo;
  int                   errNo;
} BuiltinFd;

/**
** @brief               Gets the number of arguments.
** @param argv          Array of string arguments.
** @return              Returns the number of arguments.
*/
size_t getArgc(char **argv);

#endif
