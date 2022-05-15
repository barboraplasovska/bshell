#include "echo.h"
#include "builtin.h"
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
            while (argv[i][j] == 'e' || argv[i][j] == 'n' || argv[i][j] == 'E')
            {
                if (argv[i][j] == 'n')
                    opt->nflag = true;
                if (argv[i][j] == 'e')
                    opt->eflag = true;
                if (argv[i][j] == 'E')
                    opt->Eflag = true;

                j++;
            }

            if ((argv[i][j] != '\0') || (j == 1))
                break;
        }
        else
            break;
  }
  opt->ind = i;
}

/**
** @brief               Figures out if a character is escaped.
** @param c             Character.
** @return              Returns true if character is escaped.
*/
bool isEscaped(char c)
{
    char esc[] = {'a', 'b', 'c', 'e', 'f', 'n', 'r', 't', 'v', '\\'};
    int i = 0;
    while (i < 10 && esc[i] != c)
        i++;
    
    if (i != 10)
        return true;
    return false;
}

/**
** @brief               Prints escaped characters.
** @param c             Character.
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int printEscaped(char c, BuiltinFd *builtinFd)
{
    switch (c)
    {
        case 'c':
            return -1;
        case 'e':
            fprintf(builtinFd->out, "%s", "\033");
            return 0;
	case 'n':
	    fprintf(builtinFd->out, "\n");
	    return 0;
        default:
            fprintf(builtinFd->out, "\\%c", c);
            return 0;
    }
}

/**
** @brief               Prints a word (string seperated with spaces).
** @param arg           Character array containing the "word".
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int eprintWord(char* arg, BuiltinFd *builtinFd)
{
    for (size_t i = 0; arg[i] != '\0'; i++)
    {
        if (arg[i] == '\\')
        {
            if (arg[i] != '\0')
            {
                if (isEscaped(arg[i+1]))
                {
                    if (printEscaped(arg[i+1], builtinFd) == -1)
                    {
                        fflush(builtinFd->out);
                        return -1;
                    }
                }
                else
                {
                    fprintf(builtinFd->out, "%c", arg[i]);//+1]);
                }
                i++;
            }
        }
        else
            fprintf(builtinFd->out, "%c", arg[i]);//i+1]);
    }
    fflush(builtinFd->out);
    return 0;
}

/**
** @brief               Prints the whole "word array".
** @param argv          Array of strings.
** @param argc          Number of "words" in the array.
** @param opt           Struct options defining options.
** @param builtinFd     Files.
** @return              Returns -1 in case of error.
*/
int eprintAll(char** argv, size_t argc, Options opt, BuiltinFd *builtinFd)
{
    size_t i;

    if (opt.eflag == false)
    {
        for (i = opt.ind; i < argc - 1; i++)
            fprintf(builtinFd->out, "%s ", argv[i]);
        if (argv[i])
            fprintf(builtinFd->out, "%s", argv[i]);
    }
    else
    {
        for (i = opt.ind ; i < argc-1; i++)
        {
            if (eprintWord(argv[i], builtinFd) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
            fprintf(builtinFd->out, " ");
        }

        if (argv[i])
        {
            if (eprintWord(argv[i], builtinFd) == -1)
            {
                exit(EXIT_FAILURE);
                return -1;
            }
        }
    }

    return 0;
}

/**
** @brief               Echo main function.
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of success, else -1.
*/
int echo(char** argv, BuiltinFd *builtinFd)
{
    struct Options opt;
    opt.nflag = false;
    opt.eflag = false;
    opt.Eflag = false;
    opt.ind = 0;

    size_t argc = getArgc(argv);
    if (argc != 0)
    {
        getOptions(argv, &opt, argc);

        if (eprintAll(argv, argc, opt, builtinFd) == -1)
        {
            exit(EXIT_FAILURE);
            return -1;
        }
    }

    if (opt.nflag == false)
        fprintf(builtinFd->out, "\n");
    
    fflush(builtinFd->out);
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
    AppendToHistory(argv, "echo", terminal);
    int res = echo(argv,terminal);
    free(terminal);
    return res;
}


