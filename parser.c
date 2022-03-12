#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

#include "parser.h"

const char *operators_list[2] = {"&&" , "||"};


size_t mystrlen(const char *s1)
{
    size_t i = 0;
    for(; *(s1+i);i++){}
    return i;
}

size_t get_array_size(char **args)
{
    size_t i = 0;
    //size_t size = 0;
    for(;*(args+i);i++)
    {
        //size += mystrlen(args[i]);
    }
    return i - 1;
}

size_t get_array_byte_size(char **args)
{
    size_t i = 0;
    size_t size = 0;
    for(;*(args+i);i++)
    {
        size += mystrlen(args[i]);
    }
    return size;
}


void PrintToken(struct Token *token)
{
    token = token->next;

    while(token)
    {
        printf("%s ", token->param);
        token = token->next;
    }
    printf("\n");
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

char** add_string_to_array(char **src, /*char *element*/ size_t array_size)
{
    size_t i = 0;
    size_t src_index= 1;
    char **res = (char **) malloc(array_size * sizeof(char*));
    size_t size = get_array_size(src);
    for (; i < size - 1; i++,src_index++)
    {
        if (src_index == 1)
            src_index += 1; //so skip the command
        size_t test = mystrlen(src[src_index]);
        res[i]= malloc((test+1) * sizeof(char));//mystrlen(*src[i]));
        //size_t test2 = mystrlen(res[i]);
        //size_t j= 0;
        strcpy(res[i],src[src_index]);
        /*while(j < mystrlen(src[i]))
        {
            res[i][j] = *src[j];
            j++;
        }
        //pointer_copy(*src[i],res[i],mystrlen(*src[i]));
        */
    }
    res[array_size] = NULL;
    return res;
}


char *get_string(char **args)
{
    size_t i = 1;
    char *tmp = malloc(255);
    size_t skip = 0;
    for(; args[i]; i++)
    {
        strcpy(tmp+skip,args[i]);
        skip += mystrlen(args[i]);
        *(tmp + skip) = ' ';
        skip+= 1;
    }
    *(tmp+skip) = '\n';
    skip +=1;
    char *res = malloc(i*skip);
    pointer_copy(tmp,res,i*skip);
    return res;
}


struct Token *seperate_options( char *param, struct Token *token,struct Token *prev)
{
    size_t i = 0;
    char *tmp = malloc(2);
    *tmp = *(param + i);
    *(tmp+1) = '\0';
    token->param = tmp;
    token->type = type_option;
    prev->next = token;
    token->next = NULL;
    for(i = 1; *(param + i) ; i++)
    {
        struct Token *newToken = (struct Token *)malloc(sizeof(struct Token));
        token->next = newToken;

        char *tmp = malloc(2);
        *tmp = *(param + i);
        *(tmp+1) = '\0';
        newToken->param = tmp;

        newToken->type= type_option;
        newToken->next = NULL;
        token = newToken;
    }
    return token;
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

    struct command cur_cmd;

    int i = 0;
    while(command[i])
    {
        if (command[i] == ' ' || command[i] == '\n')
        {
            char *param = (char *)malloc(paramLength);

            pointer_copy(tempParamInitial, param, paramLength);

            struct Token *token = NULL;
            token = (struct Token *)malloc(sizeof(struct Token));

            if (param[0] == '-')
            {
                token->type = type_option;
                struct Token *p_newToken;
                char *tmp_param = malloc(paramLength);
                strcpy(tmp_param,param);
                p_newToken = seperate_options(tmp_param,token,currentToken);
                currentToken->next = token;
                currentToken = p_newToken;
                token = p_newToken;
            }
            else
            {
                if (is_command(param,&cur_cmd))
                {
                    token->type = type_command;
                    token->current_command = cur_cmd;
                }
                else
                {
                    if (is_operator(param))
                        token->type = type_operators;
                    else
                        token->type = type_argument;
                }
            token->param = param;
            currentToken->next = token;

            currentToken = token;

            }
            paramLength = 0;
        }
        else
        {
            tempParamInitial[paramLength] = command[i];
            paramLength += 1;
        }

        i++;
    }

    currentToken->next = NULL;
    free(tempParamInitial);

    return initialToken;
}

int mystrcmp(char *s1, const char *s2)
{
    size_t i = 0;
    size_t s1_len = mystrlen(s1);
    size_t s2_len = mystrlen(s2);
    size_t min = (s1_len > s2_len ? s2_len : s1_len);

    for(; i < min && *(s1 +i) == *(s2+i); i++){}
    return  (i == min && (!*(s1+i) && !*(s2+i))); //second condition tells us
                                                 // if s1 is truly equal to s2
                                                 // or if they're just
                                                 // a substring
}

int is_command(char *param, struct command *cur_cmd)
{
    size_t i = 0;
    for(; i < CMDSIZE;i++)
    {
        if (mystrcmp(param,(command_list[i].param)))
                break;
    }
    if (i < CMDSIZE)
    {
        *cur_cmd = command_list[i];
        return 1;
    }
    return 0;
}


int is_operator(char *param)
{
    size_t i = 0;
    for(; i < OPSIZE;i++)
    {
        //int k = strcmp(param,operators_list[i]); only used it for debugging
        if (mystrcmp(param,(operators_list[i])))
                break;
    }
    return (i < OPSIZE);
}

int valid_option(struct Token *token, struct command current_command)
{
    int i = 0;
    if (mystrcmp(token->param,"-"))
        return 1;
    for(; i < current_command.nb_options; i++)
    {
        if (mystrcmp(current_command.valid_options[i],token->param))
            break;
    }
    return (i < current_command.nb_options);
}

int valid_input(struct Token *token)
{
    int err = 1; //instead of returning whenever we find out that the input
                // isn't valid we continue and print all the mistakes instead
                // that's why we don't return as soon as we find a mistake
    token = token->next;

    if (token->type != type_command)
        printf("command not found\n");

    struct command current_command = token->current_command;
    while(token->next)
    {
        if (token->type == type_command && token->next->type == type_command)
        {
            err = 0;
            printf("one command at a time\n");
        }
        if (token->type == type_option && !(valid_option(token,current_command)))
        {
            err = 0;
            printf("%s not a valid option for %s\n",token->param,current_command.param);
        }
        if (token->next->type == type_command)
        {
            if (token->type != type_operators)
            {
                err = 0;
                printf("add an operator before adding another command\n");
            }
            else
                current_command = token->next->current_command;
        }
        token = token->next;
    }
    return err;
}

void execute(char *argv[],struct command current_command)
{
    pid_t pid = fork();
    if (pid == -1)
        err(EXIT_FAILURE,"problem encountered while forking");
    if (!pid)
    {
        execvp(current_command.executable,argv);
    }
    wait(NULL);
}