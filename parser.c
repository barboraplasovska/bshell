#define _XOPEN_SOURCE 700
#include<stdio.h>
#include <signal.h>
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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

const char *operators_list[] = {"&&" , "||", "|", ">"};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int fd_pipe[2];

size_t mystrlen(const char *s1)
{
    size_t i = 0;
    if (!s1)
        return 0;
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


void PrintToken(struct alias *token)
{
    token = token->next;

    while(token)
    {
        printf("alias name = %s \n", *token->alias_name);
        size_t i = 1;
        printf("alias replacement = %s ", *token->alias_replacement);
        while(*(token->alias_replacement+i))
        {
            printf("%s ,", *(token->alias_replacement+i));
            i++;
        }
        printf("\n");
        token = token->next;
    }
    printf("\n");
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
        char *new = (char *) malloc((test) * sizeof(char) + 1);
        //res = (char *) realloc(res,sizeof(char) * (test) + 1);
        for(size_t j = 0; j < test; j++)
        {
            new[j] = src[src_index][j];
        }
        //strncpy(new,src[src_index],test);
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

struct alias *extract_aliases()
{
    int fd = open("./builtin/aliases.txt",O_RDONLY);
    struct alias *res = malloc(sizeof(struct alias));
    size_t len_curr = 0;
    char *curr = malloc(sizeof(char));
    struct alias *head = res;
    struct alias *prev = res;
    res->next = NULL;
    res = res->next;
    char tmp;
    while(read(fd,&tmp,1) != -1)
    {
        if(tmp == '\n')
           break;
        if(tmp == ' ')
        {
            res = malloc(sizeof(struct alias));
            prev->next = res;
            prev = prev->next;
            *(curr + len_curr) = '\0';
            res->alias_name = add_string_to_array(res->alias_name,curr);
            len_curr = 0;
            while(read(fd,&tmp,1) != -1)
            {
                if (tmp == '\n')
                {
                    *(curr+len_curr) = '\0';
                    res->alias_replacement = add_string_to_array(res->alias_replacement,curr);
                    len_curr = 0;
                    break;
                }
                if (tmp == ' ')
                {
                    *(curr+len_curr) = '\0';
                    res->alias_replacement = add_string_to_array(res->alias_replacement,curr);
                    len_curr = 0;
                }
                else
                {
                    curr = realloc(curr,sizeof(char) * (len_curr+1));
                    *(curr+len_curr) = tmp;
                    len_curr++;
                }
            }
        }
       else
       {
            curr = realloc(curr,sizeof(char) * (len_curr+1));
            *(curr+len_curr) = tmp;
            len_curr++;
       }
    }
    res->next = NULL;
    return head;
}

struct alias *is_alias(char *param, struct alias *alias)
{
    alias = alias->next;
    for(;alias && !mystrcmp(*alias->alias_name,param);alias = alias->next){}
    if(alias)
        return alias;
    return NULL;
}

char **decompose_string(char *s)
{
    char **res = malloc(sizeof(char *));
    char *word = malloc(sizeof(char));
    size_t i = 0;
    size_t res_index = 0;
    size_t word_len = 0;
    //*res = word;
    for(; *(s+i);i++)
    {
        if (*(s+i) == ' ' && word_len != 0)
        {
            //res = realloc(res,sizeof(char *) * (res_index + 2));
            word[word_len] = '\0';
            word_len = 0;
            res = add_string_to_array(res,word);
            res_index++;
            *(res + res_index) = '\0';
        }
        else
        {
            if(*(s+i) != ' ')
            {
                *(word+word_len) = *(s+i);
                word = realloc(word,sizeof(char) * word_len + 1);
                word_len++;
            }
        }
    }
    word[word_len] = '\0';
    res = add_string_to_array(res,word);
    return res;
}

char *get_string(char **args)
{
        size_t i = 1;
        char *tmp = malloc(255);
        size_t skip = 0;
        struct alias *aliases = extract_aliases();
        size_t size = 0;
        char **str_arr = args;
        if(!args[2])
        {
            char **new = decompose_string(args[1]);
            str_arr = new;
            i = 0;
        }
        for(; str_arr[i]; i++)
        {
            struct alias *alias = is_alias(str_arr[i],aliases);
            if(alias)
            {
                size_t len_s = strlen(*alias->alias_replacement);
                char *new_arg = malloc(sizeof(char) * len_s );
                size_t j = 0;
                while(*(alias->alias_replacement+j))
                {
                    //size_t size = strlen(new_arg);
                    new_arg = realloc(new_arg,sizeof(char) * size + strlen(*alias->alias_replacement + j) + 1);
                    char *curr = *(alias->alias_replacement + j);
                    for(size_t l = 0; *(curr + l); l++,size++)
                    {
                            new_arg[size] = *(curr+l);
                    }
                    new_arg[size] = ' ';
                    j++;
                    size+=1;
                }
                new_arg[size-1] = '\0';
                str_arr[i] = new_arg;
            }
                strcpy(tmp+skip,str_arr[i]);
                skip += mystrlen(str_arr[i]);
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
            token->current_command.executed = 0;

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

/*
int is_operator(char *param)
{
    size_t i = 0;
    for(; i < OPSIZE;i++)
    {
        //int k = strcmp(param,operators_list[i]); only used it for debugging
        if (mystrcmp(param,(operators_list[i])))
                break;
        return 1;
    }
    return 0;
}
*/

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
    execute(token->instruction,token->current_command,0);
    return NULL;
}


int valid_input(struct Token *token)
{
    int err = 1; //instead of returning whenever we find out that the input
                // isn't valid we continue and print all the mistakes instead
                // that's why we don't return as soon as we find a mistake
    token = token->next;

    pipe(fd_pipe);
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
    //pthread_t thr;

    char opts[10];

    pid_t pid = 0;
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

                    if (strlen(opts) == 2)
                    {
                        token->instruction = add_string_to_array(token->instruction,opts);
                        opts[0] = '\0';
                        //token->next->instruction = token->instruction;
                    }
                    else
                    {
                        if(opts[0] != '-')
                        {
                            opts[0] = '-';
                            strncat(opts,token->param,1);
                            token->instruction = add_string_to_array(token->instruction,opts);
                            //token->next->instruction = token->instruction;
                        }
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
            pid_t second_fork = 0;
            if(strcmp(Token_param,"|") == 0)
            {
                pid = fork();
                if (pid == 0)
                {
                    if(token->next && token->next->type == type_command)
                    {
                        dup2(fd_pipe[0],STDIN_FILENO);
                        close(fd_pipe[1]);
                        struct Token *tmp = token->next;
                        char *options = calloc(sizeof(char),2);
                        size_t k = 0;
                        while(tmp->next && tmp->next->type != type_operators)
                        {
                            if(tmp->next->type == type_option)
                            {
                                if(k == 1)
                                {
                                    *(options + k) = tmp->next->param[0];
                                    k = 0;
                                    token->next->instruction = add_string_to_array(token->next->instruction, options);
                                    options = calloc(sizeof(char),2);
                                }
                                else
                                {
                                    if (k == 0 && tmp->next->param[0] != '-')
                                    {
                                        *(options) = '-';
                                        *(options+1) = tmp->next->param[0];
                                        token->next->instruction = add_string_to_array(token->next->instruction, options);
                                        options = calloc(sizeof(char),2);
                                        k = 0;
                                    }
                                    else
                                    {
                                        *(options+k) = tmp->next->param[k];
                                        k+=1;
                                    }
                                }
                            }
                            else
                            {
                                token->next->instruction = add_string_to_array(token->next->instruction, tmp->next->param);
                            }
                            tmp = tmp->next;
                        }
                        res = execvp(token->next->current_command.executable,token->next->instruction);
                    }
                }
                else
                {
                    close(fd_pipe[0]);
                    token->next->current_command.executed = 1;
                    second_fork = fork();
                    if (second_fork == 0)
                    {
                        if(token->next && token->next->type == type_command)
                        {
                            dup2(fd_pipe[1],STDOUT_FILENO);
                            close(fd_pipe[0]);
                            execvp(token->current_command.executable,token->instruction);
                        }
                    }
                    else
                    {
                        close(fd_pipe[1]);
                        waitpid(second_fork,0,0);
                    }
                }


                waitpid(pid,0,0);
                waitpid(second_fork,0,0);
                kill(second_fork,SIGKILL);
                kill(pid,SIGKILL);
            }
            if((strcmp(Token_param,">") == 0 ))
            {
                if(token->next)
                {
                    int fd;
                    fd = open(token->next->param,O_APPEND | O_CREAT | O_WRONLY | O_TRUNC, 0666);
                    if(!fork())
                    {
                        if(dup2(fd,STDOUT_FILENO) < 0)
                            printf("dup2 error\n");

                        close(fd);
                        //ttoken->current_command.executed = 1;
                        res = execvp(token->current_command.executable,token->instruction);//execute the first command

                    }
                    wait(NULL);

                    current_command.executed = 1;
                    current_command.executable[0] = '\0';
                    //err = 0;
                    res = 0;
                }
                else
                {
                    printf("syntax error: expected token after > and got nothing\n");
                }
            }

            if(strcmp(Token_param,"||") == 0)
            {
                if (err == 1 && res==0 && !current_command.executed)
                {
                    res = execute(token->instruction,token->current_command,0);
                    if(res == 0)
                        current_command.executed = 1;
                    pthread_cond_signal(&cond);
                }
                //err = 1; //reset the err value

                if(current_command.executed)
                {
                    while(token->next && token->next->type != type_operators )//&& token->next->type != type_command)
                    {
                        token = token->next;
                        current_command.executed = 1;
                    }
                }
                if (res == 0) //&& current_command.executable[0] != '\0')
                {
                    if(current_command.executable[0] != '\0' && !current_command.executed)
                    {
                        if (err == 1)
                        {
                            if(token->next)
                                token = token->next;
                            while(token->next && token->type != type_operators)
                            {
                                    token = token->next;
                            }
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
                    }
                    /*else
                    {
                        if((err == 1 && res == 0)) //|| token->next->type != type_command)
                            break;
                    }*/
                    //err = 1; //reset the err value

                }

            }
            if(strcmp(Token_param,"&&") == 0 )
            {
                if (err == 1 && current_command.executable[0] != '\0' && !current_command.executed)
                {
                    current_command.executed = 1;
                    res = execute(token->instruction,token->current_command,0);
                    pthread_cond_signal(&cond);
                }
                if (res != 0 )//|| err == 0)
                {
                    break;
                }
                /*if (token->next)
                {
                    if (token->next->type != type_command)
                        printf("%s: command not found\n",token->next->param);
                }
                */

                current_command.executable[0]= '\0';
                current_command.executed = 0;
            }
            if(token->next)
            {
                char Token_next_param[mystrlen(token->next->param)];
                strncpy(Token_next_param,token->next->param,mystrlen(token->next->param));
                Token_next_param[mystrlen(token->next->param)] = '\0';

                if(token->next->type != type_command)
                {
                    if(strcmp(Token_param,">") != 0 && !current_command.executed)
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
            //current_command.executable[0]= '\0';
            token->instruction = NULL;
        }
        if (!token->next && token->type != type_operators && token->current_command.executable[0] != '\0')
        {
            if (err == 1 && current_command.executed != 1)
            {
                current_command.executed = 1;
                res = execute(token->instruction,token->current_command,0);
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

int execute(char *argv[],struct command current_command, pid_t pid_cmd)
{
    int res = 0;
    pid_cmd = fork();
    if (pid_cmd == -1)
        err(EXIT_FAILURE,"problem encountered while forking");
    if (!pid_cmd)
    {
        /*char buff[256];
        int fd_fifo = open("/tmp/pipe",O_RDWR | O_APPEND);
        int r = read(fd_fifo,buff,256);
        int wrote =  write(fd_fifo,"h\0",2);
        if ( wrote > 0  && r > 0)
        {
            //close(fd_pipe[1]);
            //dup2(fd_pipe[0],STDIN_FILENO);
            //close(fd_pipe[0]);
            argv = NULL;
            argv = add_string_to_array(argv,"/tmp/pipe");
            //argv = add_string_to_array(argv,NULL);
            //close(fd_pipe[0]);
            if(fd_fifo)
            {
                execvp(current_command.executable,argv);
            }
        }
        else
        {*/
            res = execvp(current_command.executable,argv);
        //}
    }

    //close(fd_fifo);
    waitpid(-1,&res,0);//res will be different from 0 if we reached a part in
                       //the code of any command where we exit(EXIT_FAILURE)
    //unlink("/tmp/pipe");
    /*if(pid_cmd != 0)
    {
        kill(pid_cmd,SIGKILL);
    }*/
    return res;
    /*for(;i >= 0; i--)
    {
        p = instruction[i];
        free(p);
        p = NULL;
    }*/
}
int free_tokens(struct Token *token)
{

    int res = 5;
    size_t second = 0;
    while (token)
    {
        struct Token *p = token->next;
        //free(token);
        free(token->param);
        //token->param = NULL;
        if(token->instruction)
        {
            //if(*token->instruction+1)
            /*if(second == 0 && *(token->instruction+1))
            {
                second = 1;
                free_instruction(token->instruction,second);
                free(token);
                token = p;
                continue;
            }*/
            //res = free_instruction(token->instruction,second);

            if(res == 1 && second == 1)
            {
                free(token);
                token = p;
                second++;
                continue;
            }
            else
            {
                free(token->instruction);
                token->instruction = NULL;
            }
            second++;
        }
        else
        {
            second = 0;
        }
        //free(token->instruction);
        //token->instruction = NULL;
                    //}
        free(token);
        token = p;
    }
    return 0;
}
