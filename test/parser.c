#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include  <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include "parser.h"

const char *operators_list[] = {"&&" , "||", "|", ">"};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

size_t mystrlen(const char *s1)
{
    size_t i = 0;
    for(; *(s1+i);i++){}
    return i;
}

size_t get_array_size(char **args)
{
    size_t i = 0;
    for(;*(args+i);i++){};
    return i;
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

char** add_string_to_array(char **src, char *element)
{
    size_t i = 0;
    size_t src_index= 0;

    size_t l = 0;
    char *res = NULL;
    size_t size = 0;
    if (src)
        size = get_array_size(src);
    l = size+1;
    char **p_res = (char **) malloc((l) * sizeof(char*));
    for (; i < size ; i++,src_index++)
    {
        size_t test = strlen(src[src_index]);
        char *new = NULL;
        new = (char *) malloc((test) * sizeof(char) + 1);
        //res = (char *) realloc(res,sizeof(char) * (test) + 1);
        strncpy(new,src[src_index],test);
        *(new+test) = '\0';
        (p_res[i]) = new;
    }
    size_t element_len = mystrlen(element);
    res = (char *) malloc(sizeof(char) * mystrlen(element) + 1);
    strncpy(res,element,mystrlen(element));

    *(res+element_len) = '\0';
    (p_res[size]) = res;
    *(p_res+size+1) = '\0';
    return p_res;
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
        char *res = malloc(skip);
        pointer_copy(tmp,res,skip-1);
        res[skip] = '\0';
        free(tmp);
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
        initialToken->instruction = NULL;

        struct Token *currentToken = initialToken;

        char *tempParamInitial;
        currentToken->instruction = NULL;
        tempParamInitial = (char *)malloc(255);
        size_t paramLength = 0;

        struct command cur_cmd;

        int i = 0;
        int len_args = strlen(command);
    //    while(command[i])
        while(i <= len_args)
        {
            if (command[i] == ' ' || command[i] == '\n' || command[i] == '\0')
            {
                char *param = (char *)malloc(paramLength);

                pointer_copy(tempParamInitial, param, paramLength);

                struct Token *token = NULL;
                token = (struct Token *)malloc(sizeof(struct Token));
                token->instruction = NULL;

                if (param[0] == '-')
                {
                    token->type = type_option;
                    struct Token *p_newToken;
                    char *tmp_param = malloc(paramLength);
                    strncpy(tmp_param,param,paramLength);
                    p_newToken = seperate_options(tmp_param,token,currentToken);
                    currentToken->next = token;
                    currentToken = p_newToken;
                    token = p_newToken;
                    free(tmp_param);
                    free(param);
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
                token->param = NULL;//(char *) malloc(paramLength);
                token->param = param;
                currentToken->next = token;

                currentToken = token;
                //free(token);
                }
                paramLength = 0;
                //free(param);
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

    void* worker(void* arg)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        pthread_mutex_unlock(&mutex);
        struct Token *token = arg;
        execute(token->instruction,token->current_command);
        return NULL;
    }


    int valid_input(struct Token *token)
    {
        int err = 1; //instead of returning whenever we find out that the input
                    // isn't valid we continue and print all the mistakes instead
                    // that's why we don't return as soon as we find a mistake
        token = token->next;

        struct command current_command = token->current_command;

        if (token->type != type_command)
        {
            current_command.executable[0] = '\0';
            char Token_param[mystrlen(token->param) + 1];
            strncpy(Token_param,token->param,mystrlen(token->param));
            Token_param[mystrlen(token->param)] = '\0';
            printf("%s: command not found\n",Token_param);
            err = 0;
            current_command.args_needed = -1;
        }
        int nb_args = 0;
        int res = 1; //return value of the current command that's being executed
        pthread_t thr;

        char opts[10];

        while(token)
        {
            if (token->type == type_command)
            {
                err = 1;
                res = 0;
                if (token->next && token->next->type == type_command)
                {
                    err = 0;
                    printf("one command at a time\n");
                }
            }

            if (token->type == type_argument)
            {
                nb_args++;
                if (nb_args > current_command.args_needed && current_command.args_needed != -1
                    && current_command.param[0] != '\0')
                {
                    err = 0;
                    nb_args = -99; //temporary fix so it prints once not for every
                                   //extra argument
                    printf("too many arguments for %s\n",current_command.param);
                }
                else
                {
                    token->instruction = add_string_to_array(token->instruction,token->param);
                    if(token->next)
                        token->next->instruction = token->instruction;
                }
            }
            if(token->type == type_option)
            {
                if (!(valid_option(token,current_command)))
                {
                    err = 0;
                    printf("%s not a valid option for %s\n",token->param,current_command.param);
                }
                else
                {
                    if (token->next)
                    {
                        strncat(opts,token->param,1);

                        if (token->next->type != type_option)
                        {
                            token->instruction = add_string_to_array(token->instruction,opts);
                        }
                        token->next->instruction = token->instruction;
                    }
                }
            }
            if (token->next && token->next->type == type_command)
            {
                if (token->type != type_operators)
                {
                    err = 0;
                    printf("add an operator before adding another command\n");
                }
            }
            if(token->type == type_operators)
            {
                char Token_param[mystrlen(token->param) + 1];
                strncpy(Token_param,token->param,mystrlen(token->param));
                Token_param[mystrlen(token->param)] = '\0';
                if(strcmp(Token_param,"|") == 0)
                {
                        if (pthread_create(&thr,NULL,worker,(void *)token) != 0)
                        {
                            return err;
                        }
                        //pthread_join(thr,NULL);
                        token= token->next;
                        continue;
                }
                if((strcmp(Token_param,">") == 0 ) && token->next)
                {
                    //token->next->instruction = NULL;
                    token->next->instruction = add_string_to_array(token->next->instruction,token->next->param);
                    int fd;
                    fd = open(*token->next->instruction,O_APPEND | O_CREAT | O_WRONLY | O_TRUNC, 0666);
            //        token->next->instruction = NULL;
                    if(!fork())
                    {
                        if(dup2(fd,STDOUT_FILENO) < 0)
                            printf("rer");

                        close(fd);
                        res = execvp(token->current_command.executable,token->instruction);//execute the first command

                    }
                    wait(NULL);

                    err = 0;
                    res = 0;
                }

                if(strcmp(Token_param,"||") == 0)
                {
                    if (err == 1 && res==0)
                    {
                        res = execute(token->instruction,token->current_command);
                        pthread_cond_signal(&cond);
                    }
                    err = 1; //reset the err value
                    if (res == 0 && current_command.executable[0] != '\0')
                    {
                        if(token->next)
                            token = token->next;
                        while(token->next && token->type != type_operators)
                        {
                                token = token->next;
                        }

                        if(token->next)
                        {

                            if(token->next->type != type_command)
                            {
                                strncpy(Token_param,token->param,mystrlen(token->param));
                                Token_param[mystrlen(token->param)] = '\0';
                                current_command.executable[0] = '\0';
                                err = 0;
                            }
                        }
                        else
                        {
                            if(res == 0)
                                break;
                        }

                    }
                }
                if(strcmp(Token_param,"&&") == 0 )
                {
                    if (err == 1)
                    {
                        res = execute(token->instruction,token->current_command);
                        pthread_cond_signal(&cond);
                    }
                    if (res != 0 )//|| err == 0)
                    {
                        break;
                    }

                    current_command.executable[0]= '\0';
                }
                if(token->next)
                {
                    char Token_next_param[mystrlen(token->next->param)];
                    strncpy(Token_next_param,token->next->param,mystrlen(token->next->param));
                    Token_next_param[mystrlen(token->next->param)] = '\0';

                    if(token->next->type != type_command)
                    {
                        if(strcmp(Token_param,">") != 0)
                            printf("%s: command not found\n",Token_next_param);
                        if (strcmp(Token_param,"&&") == 0)
                        {
                            err = 0;
                            res = 1;
                        }
                    }
                    if (token->next->type == type_command)
                        current_command = token->next->current_command;
                }
                token->instruction = NULL;
            }
            if (!token->next && token->type != type_operators && token->current_command.executable[0] != '\0')
            {
                if (err == 1)
                {
                    res = execute(token->instruction,token->current_command);
                    pthread_cond_signal(&cond);
                }
                else
                    res = -1;
            }

            token = token->next;
            if(token)
                token->current_command = current_command;
        }
        return err;
    }

    int execute(char *argv[],struct command current_command)
    {
        int res = 0;
        pid_t pid = fork();
        if (pid == -1)
            err(EXIT_FAILURE,"problem encountered while forking");
        if (!pid)
        {
                execvp(current_command.executable,argv);
        }
        waitpid(-1,&res,0);//res will be different from 0 if we reached a part in
                           //the code of any command where we exit(EXIT_FAILURE)
        return res;
    }

    void free_instruction(char **instruction)
    {
        char *p = NULL;
        ssize_t i =0;
        for(; instruction[i]; i++)
        {
            p = instruction[i];
            free(p);
        }
        //i-=1;
        /*for(;i >= 0; i--)
        {
            p = instruction[i];
            free(p);
            p = NULL;
        }*/
    }
    int free_tokens(struct Token *token)
    {
        while (token)
        {
            struct Token *p = token->next;
            free(token);
            free(token->param);
            token->param = NULL;
            if(token->instruction)// && !token->next)
            {
                //if(*token->instruction+1)
                free_instruction(token->instruction);
                free(token->instruction);
                token->instruction = NULL;

            }
            free(token->instruction);
            token->instruction = NULL;
                        //}
            token = p;
        }
        return 0;
}
