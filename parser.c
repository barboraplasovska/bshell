#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"

void PrintToken(struct Token *token)
{
    token = token->next;

    while(token)
    {
        printf("%s", token->param);
        token = token->next;
    }
}

void pointer_copy(char *source, char *destination, size_t length)
{
    for(size_t count = 0; count < length; count++)
    {
        *destination = *source;
        source++;
        destination++;
    }

    *destination = '\0';
}

struct Token *getParameters(const char *command)
{
    struct Token *initialToken = NULL;
    initialToken = (struct Token *)malloc(sizeof(struct Token)); // used as sentinel
    initialToken->param = NULL;
    initialToken->next = NULL;

    struct Token *currentToken = initialToken;

    char *tempParamInitial;
    tempParamInitial = (char *)malloc(255);
    size_t paramLength = 0;

    int i = 0;
    while(command[i])
    {
        if (isspace(command[i] || command[i] == '\n'))
        {
            struct Token *token = NULL;
            token = (struct Token *)malloc(sizeof(struct Token));
            char *param = (char *)malloc(paramLength);

            pointer_copy(tempParamInitial, param, paramLength);

            token->param = param;
            currentToken->next = token;

            currentToken = token;

            paramLength = 0;
        }
        else
        {
            tempParamInitial[paramLength] = command[i];
            paramLength += 1;
        }

        i++;
    }
    
    //fix: repeated code from inside loop here
    struct Token *token = NULL;
    token = (struct Token *)malloc(sizeof(struct Token));
    char *param = (char *)malloc(paramLength);

    pointer_copy(tempParamInitial, param, paramLength);

    token->param = param;
    initialToken->next = token;

    paramLength = 0;

    free(tempParamInitial);

    return initialToken;
}