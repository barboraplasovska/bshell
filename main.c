#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc, char** argv)
{
    const char *command = "cd -aeb -e && file2 || fil3 echo\n";
    //const char *command = "";//ls -a -b file1 && file2 || fil3 echo\n";
    char *input = get_string(argv);
    struct Token *token = getParameters(input);
    //char **res = (char **) malloc(array_size * sizeof(char*));
    int valid = valid_input(token);//,res);
    if(valid)
    {
        size_t arr_size = get_array_size(argv);
        argv[0] = token->next->current_command.executable; //first token is a sentinel
        size_t byte_size = get_array_byte_size(argv);
        char **res = add_string_to_array(argv,NULL,byte_size);
        execute(res,token->next->current_command);
    }
    PrintToken(token);
}
