#ifndef CPUI_H
#define CPUI_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"


/**
** @brief               Gets the information on the cpu
** @param builtinFd     Files.
*/
char* GetCPUInfo(BuiltinFd *builtinFd);

#endif