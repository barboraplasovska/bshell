#ifndef PRCK_H
#define PRCK_H

#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"


/**
** @brief               Kill process from pid.
** @param builtinFd     Files.
*/

void procKill(pid_t pid ,BuiltinFd *builtinFd);

#endif
