#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#define CMDSIZE 9
#define OPSIZE 2

enum type
{
    type_command,
    type_option,
    type_argument,
    type_operators,
};

struct command
{
    char param[15];
    char *valid_options[3];
    char executable[15];
    int args_needed;
    int nb_options;
};


struct Token {
    enum type type;
    char* param;
    struct Token* next;
    struct command current_command;
};

static const struct command ls = { "ls", {"a","h"}, "./ls", 1 , 2};
static const struct command cat = { "cat", {"e","h"}, "./cat", 1 , 2};
static const struct command cd = { "cd", {"L","e"}, "./cd", 0 , 2};
static const struct command echo = { "echo", {"n","e","E"}, "./echo", -1 ,3};
static const struct command clear = { "clear", {"n","e","E"}, "./clear", -1 ,3};

static const struct command cp = { "cp", {}, "./cp", -1 , 0};
static const struct command procstatus = { "procstatus", {}, "./procstatus", -1 , 0};

static const struct command touch = { "touch", {}, "./touch", -1 ,0};
static const struct command mv = {"mv", {}, "./mv", -1, 0};


static const struct command command_list[CMDSIZE] =
{ls, cd, echo, cat, clear,cp, touch, mv,procstatus};



size_t get_array_size(char **args);

size_t get_array_byte_size(char **args);

char** add_string_to_array(char **src, /*char *element,*/ size_t array_size);

char *get_string(char**args);

//const char *operators_list[2] = {"&&" , "||"};

void PrintToken(struct Token *token);

void pointer_copy(char *source, char *destination, size_t length);

struct Token *getParameters(const char* command);

int is_command(char *param,struct command *current_command);

int is_operator(char *param);

int valid_input(struct Token *token);

void execute(char *argv[], struct command current_command);
#endif
