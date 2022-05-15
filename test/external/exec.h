#ifndef EXEC_H
#define EXEC_H
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include<err.h>
#include <sys/wait.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"

/**
** @brief               Runs exec on argument
** @param argv          parameter.
** @param builtinFd     Files.
** @return              -1 on failure, 0 on success
*/
int exec(char **argv, BuiltinFd *builtinFd);

#endif
