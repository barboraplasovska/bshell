#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc,char** argv)
{
    if (argc == 0)
        return 0;
    if(argv[1]== NULL)
        return 0;

    char *input = get_string(argv);
    struct Token *token = getParameters(input);
    int valid = valid_input(token);

    //free_tokens(token);
    //free(input);
    if(valid)
    {
        //argv[0] = token->next->current_command.executable; //first token is a sentinel
        //char **res = add_string_to_array(argv,NULL);
        //execute(res,token->next->current_command);
    }
    return 0;
}
