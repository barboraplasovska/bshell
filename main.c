#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv)
{
    const char *command = "ls -a -b\n";
    struct Token *token = getParameters(command);
    PrintToken(token);
}
