#include <unistd.h> 
#include "bsh.h"


char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "bsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\n';
      buffer[position+1] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "bsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


void print_prompt(void){
    //char hostname[1024];
    //char user[1024];
    char* r;// = gethostname(hostname,1024);
    /*if (r == -1){
        err(-1,"gethostname");
    }*/
    r = getlogin();
    /*if (r != 0){
        err(-2,"getlogin_r");
    }*/
    //size_t size = strlen(hostname) + strlen(user); //add colors and a place to edit that 
    //char *prompt_1 = strncat(hostname,"@",size);
    //char *prompt_2 = strncat(prompt_1,user,size);
    //red();
    printf("\033[1;31m");
    printf(r);
    //reset();
    printf("\033[0m");
    printf("@");
    //red();
    printf("\033[1;31m");
    printf("bsh");
    //reset();
    printf("\033[0m");
    printf("~");
}

void loop(void){
    //char *line;
    //char **args;
    int status = 1;

    do{
        print_prompt();
        
        char *input = lsh_read_line();
        struct Token *token = getParameters(input);
        //args = lsh_split_line(input);
        int valid = valid_input(token);
        free(token);
        //status = valid;
        if (valid){ //do not consider ./main -> to fix
            //args[0] = token->next->current_command.executable;
//            size_t byte_size = get_array_byte_size(args);
  //          char **res = add_string_to_array(args,byte_size);
   //         execute(res,token->next->current_command);
        }
    } while (status);

}

int main(void)
{
    loop();

    return EXIT_SUCCESS;
}
