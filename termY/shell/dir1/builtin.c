#include "builtin.h"

/**
** @brief               Gets the number of arguments.
** @param argv          Array of string arguments.
** @return              Returns the number of arguments.
*/
size_t getArgc(char **argv)
{
    size_t i = 0;
    while (argv[i] != NULL)
        i++;

    return i;
}