#ifndef ECHO_H
#define ECHO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "builtin.h"


/**
** @brief               Builtin echo options.
** @param nflag         Option -n is activated.
** @param eflag         Option -e is activated.
** @param Eflag         Option -E is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct                  Options
{
  bool                  nflag;
  bool                  eflag;
  bool                  Eflag;
  size_t                ind;
} Options;

/**
** @brief               Gets the number of arguments.
** @param argv          Array of string arguments.
** @return              Returns the number of arguments.
*/
size_t getArgc(char **argv);

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc);

/**
** @brief               Figures out if a character is escaped.
** @param c             Character.
** @return              Returns true if character is escaped.
*/
bool isEscaped(char c);

/**
** @brief               Prints escaped characters.
** @param c             Character.
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int printEscaped(char c, BuiltinFd *builtinFd);

/**
** @brief               Prints a word (string seperated with spaces).
** @param arg           Character array containing the "word".
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int eprintWord(char* arg, BuiltinFd *builtinFd);

/**
** @brief               Prints the whole "word array".
** @param argv          Array of strings.
** @param argc          Number of "words" in the array.
** @param opt           Struct options defining options.
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int eprintAll(char** argv, size_t argc, Options opt, BuiltinFd *builtinFd);

/**
** @brief               Echo main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int echo(char** argv, BuiltinFd *builtinFd);

#endif