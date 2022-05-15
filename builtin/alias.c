#include "alias.h"

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc)
{
    size_t i, j = 0;//1 
    for (i = 0; i < argc; i++) //1
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'p')
            {
                if (argv[i][j] == 'p')
                    opt->pflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }

  if (i < argc)
  	opt->ind = i;
}

int printAliases(BuiltinFd* builtinFd)
{
   char ch;
   FILE *file;

   file = fopen("./builtin/aliases.txt", "r+");

   if (file == NULL)
        return -1;

   while ((ch = fgetc(file)) != EOF)
        fprintf(builtinFd->out, "%c", ch);

   fclose(file);

   return 0;
}

int addAlias (char* command, char* alias)
{
    bool replaced = false;
    char line[BUFFER_SIZE];
    FILE *source;
    FILE *target;
    source = fopen("./builtin/aliases.txt", "r+");
    target = fopen("./builtin/temp.txt", "w+");

    if (source == NULL)
            return -1;
    
    while (fgets(line, BUFFER_SIZE, source) != NULL)
    {
        char word[strlen(line)];
        for (size_t i = 0; i < strlen(line) && line[i] != ' '; i++)
            word[i] = line[i];

        if (strcmp(word,command))
        {
            replaced = true;
        }

        if (!replaced)
            fprintf(target, "%s",line);
        else
        {
            fprintf(target, "%s %s\n", command, alias); 
        }

    }

    if (!replaced)
       fprintf(target, "%s %s\n", command, alias); 

    fclose(source);
    fclose(target);

    remove("./builtin/aliases.txt");
    rename("./builtin/temp.txt", "./builtin/aliases.txt");
    return 0;
}

int addAliases(char** argv, Options opt, size_t argc, BuiltinFd* builtinFd)
{
    
    for (size_t i = opt.ind; i < argc; i++)
    {

        char* command = calloc(strlen(argv[i]), sizeof(char));
        char* alias = calloc(strlen(argv[i]), sizeof(char));

        size_t j = 0;
        char* temp = argv[i];
        while (temp[j] != '=' && temp[j] != '\0')
        {
            command[j] = temp[j];
            j++;
        }
        command[j] = '\0';

        if (temp[j] == '\0' || temp[j+1] != '\'') // wrong format
        {
	    fprintf(builtinFd->err, "alias: wrong format\n");
            fprintf(builtinFd->err, "alias: usage: alias [-p] [name[=\"value\"] ... ]\n");
            return -1;
        }
        j += 2; //skipping = and "

        size_t k = 0;
        while(temp[j] != '\'' && temp[j] != '\0')
        {

            alias[k] = temp[j];
            j++;
            k++;
        }
        alias[k] = '\0';

        if (temp[j] == '\0')
        {
            fprintf(builtinFd->err, "alias: wrong format\n");
            fprintf(builtinFd->err, "alias: usage: alias [-p] [name[=\"value\"] ... ]\n");
            return -1;
        }

        if (addAlias(command, alias) != 0)
        {
            free(command);
            free(alias);
            return -1;
        }
        free(command);
        free(alias);
    }
    return 0;
}

/**
** @brief               tail main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int alias(char** argv, BuiltinFd *builtinFd)
{
    Options opt;
    opt.pflag = false;
    opt.ind = -1;

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "alias: missing parameter\n");
        fprintf(builtinFd->err, "alias: usage: alias [-p] [name[=\"value\"] ... ]\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        getOptions(argv, &opt, argc);

        if (opt.pflag) // print aliases
        {
            if (printAliases(builtinFd) != 0)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        if (argc > 1 || !opt.pflag) // add alias
        {
	    for (size_t i = 0; i < argc; i++)
            if (addAliases(argv, opt, argc, builtinFd) != 0)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }

    return 0;
}


int main(int argc, char **argv)
{
    if (argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "alias", terminal);
    alias(argv,terminal);
    free(terminal);
}
