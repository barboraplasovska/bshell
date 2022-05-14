#include "unalias.h"


int removeAlias(char** argv,  size_t argc)
{
    char line[BUFFER_SIZE];
    FILE *source;
    FILE *target;
    source = fopen("aliases.txt", "r+");
    target = fopen("temp.txt", "w+");

    if (source == NULL)
            return -1;
    
    while (fgets(line, BUFFER_SIZE, source) != NULL)
    {
        bool todelete = false;
        for (size_t i = 0; i < argc; i++)
        {
            if (strcmp(line, argv[i]))
            {
                todelete = true;
                break;
            }
        }

        if (!todelete)
            fprintf(target, "%s", line);
    }

    fclose(source);
    fclose(target);

    remove("aliases.txt");
    rename("temp.txt", "aliases.txt");

    return 0;
}

/**
** @brief               tail main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int unalias(char** argv, BuiltinFd *builtinFd)
{

    size_t argc = getArgc(argv);
    if (argc == 0)
    {
        fprintf(builtinFd->err, "unalias: missing parameter\n");
        fprintf(builtinFd->err, "unalias: usage: unalias [alias name]\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    else
    {
        if (removeAlias(argv, argc) != 0)
        {
            exit(EXIT_FAILURE);
            return -1;
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
    AppendToHistory(argv, "unalias", terminal);
    unalias(argv,terminal);
    free(terminal);
}
