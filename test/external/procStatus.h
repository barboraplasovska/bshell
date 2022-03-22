#ifndef PRCS_H
#define PRCS_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"


/**
** @brief               Get the process status from the process ID.
** @param pid           Process ID.
** @param builtinFd     Files.
** @return              Returns the status of the process
*/
char* get_process_status_by_pid(const int pid, BuiltinFd *builtinFd);

#endif
