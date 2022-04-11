#ifndef BSH_H
#define BSH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/wait.h>

#include "parser.h"
                                                        
#define LSH_TOK_BUFSIZE 64                                                      
#define LSH_TOK_DELIM " \t\r\n\a"                                               
#define LSH_RL_BUFSIZE 1024

void loop(void); //main shell loop
void print_prompt(void); //prompt -> to change when customization come
char **lsh_split_line(char *line); // splits line parsed into array of string
// ex: {"ls","-a"}
char *lsh_read_line(void); //reads line from main stdin of shell



#endif
