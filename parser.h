#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

//char commands[][4] = {"ls", "cd", "echo"};

struct Token {
    char* param;
    struct Token* next;
};

void PrintToken(struct Token *token);

void pointer_copy(char *source, char *destination, size_t length);

struct Token *getParameters(const char* command);

#endif