#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#define CMDSIZE 20
#define OPSIZE 4

enum type
{
    type_command,
    type_option,
    type_argument,
    type_operators,
};

struct alias
{
    char **alias_name;
    char **alias_replacement;
    struct alias *next;
};
struct command
{
    char param[15];
    char *valid_options[7];
    char executable[15];
    int args_needed;
    int nb_options;
    int executed;
};


struct Token {
    enum type type;
    char *param;
    struct Token *next;
    struct command current_command;
    char **instruction;
};

// BUILTIN
static const struct command cat = 
        {"cat", {"A", "E", "s", "n"}, "./cat", -1 , 4, 0};
static const struct command clear = { "clear", {}, "./clear", 0 ,0, 0};
static const struct command clearhistory = { "clearhistory", {}, "./clearhistory", 0 ,0 ,0};
static const struct command cp = { "cp", {}, "./cp", -1 , 0, 0};
static const struct command date = { "date", {}, "./date", 0 , 0, 0};
static const struct command echo = { "echo", {"n","e","E"}, "./echo", -1 ,3, 0};
static const struct command grep = { "grep", {"n","i"}, "./grep", -1,2, 0};
static const struct command help = { "help", {}, "./help", 0 , 0, 0};
static const struct command ls = { "ls", {}, "./ls", -1 , 0, 0};
static const struct command mkdirr = { "mkdir", {}, "./mkdir", -1 ,0, 0};
static const struct command mv = {"mv", {}, "./mv", -1, -1, 0};
static const struct command rm = {"rm", {"i", "f"}, "./rm", -1, 2, 0}; //-r
//static const struct command rmdirr = {"rmdir", {}, "./rmdir", -1, 0};
static const struct command sort = 
        {"sort", {"o","r","n","c","u","M"}, "./sort", -1, 6, 0};
static const struct command tac = {"tac", {"n"}, "./tac", -1, 1, 0};
static const struct command touch = { "touch", {}, "./touch", -1 ,-1, 0};

// EXTERNAL
static const struct command cpuinfo = 
        { "cpuinfo", {}, "./cpuinfo", -1 , -1, 0};
static const struct command history = 
        { "history", {}, "./history", -1 , -1, 0};
static const struct command prockill = 
        { "prockill", {}, "./prockill", -1 , -1, 0};
static const struct command proclist = 
        { "proclist", {}, "./proclist", -1 , -1, 0};
static const struct command procstatus = 
        { "procstatus", {}, "./procstatus", -1 , -1, 0};



static const struct command command_list[CMDSIZE] =
{cat, clear, cp, date, echo, grep, help, ls, mkdirr, mv, rm, sort, tac,
touch, cpuinfo, history, prockill, proclist, procstatus, clearhistory};



size_t get_array_size(char **args);

size_t get_array_byte_size(char **args);
char** add_string_to_array(char **src, char *element);

char *get_string(char**args);

void PrintToken(struct alias *token);

void pointer_copy(char *source, char *destination, size_t length);

struct Token *getParameters(const char* command);

int is_command(char *param,struct command *current_command);

int is_operator(char *param);

int valid_input(struct Token *token);

struct alias *extract_aliases();

int execute(char *argv[], struct command current_command,pid_t pid_cmd);
#endif
