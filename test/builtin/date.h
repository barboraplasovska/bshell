#ifndef DATE_H
#define DATE_H

#include "builtin.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

/**
** @brief               Gives the current system date.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int date(BuiltinFd *builtinFd);

#endif