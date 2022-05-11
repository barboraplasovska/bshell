#ifndef HISTORY_H
#define HISTORY_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../builtin/builtin.h"

struct Node {
    struct Node* next;
    struct Node* prev;
    char* array;
};

/**
** @brief               goes through the history of the commands and allows to call them
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int history(BuiltinFd *builtinFd);

#endif