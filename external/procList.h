#ifndef LPRC_H
#define LPRC_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
//#include <sys/stat.h>

#include "../builtin/builtin.h"


/**
** @brief               Lists all the process ids
** @param builtinFd     Files.
** @return              Returns the char double pointer of all the process ids running
*/

char** listProc(BuiltinFd *builtinFd);

#endif