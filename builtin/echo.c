#include "echo.h"

/**
** @brief               Gets the options
** @param argv          Array of string arguments.
** @param opt           Options structure.
** @param argc          The number of arguments.
*/
void getOptions(char** argv, Options* opt, size_t argc)
{
    size_t i, j = 1;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            j = 1;
            while (argv[i][j] == 'n')
            {
                if (argv[i][j] == 'n')
                    opt->nflag = true;

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
                    if (printEscaped(arg[i+1]) == -1)
                    {
                        fflush(builtinFd->out);
                        return -1;
                    }
                }
                else
                {
                    fprintf(builtinFd->out, "%c", arg[i+1]);
                }
                i++;
            }
        }
        else
            fprintf(builtinFd->out, "%c", arg[i+1]);
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
    for (i = opt.ind; i < argc-1; i++)
    {
        if (eprintWord(argv[i]) == -1)
            return -1;
        fprintf(builtinFd->out, " ");
    }

    if (argv[i])
    {
        if (eprintWord(argv[i]) == -1)
            return -1;
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
    opt.ind = 0;

    size_t argc = getArgc(argv);
    
    getOptions(argv, &opt, argc);

    if (eprintAll(argv, argc, opt) == -1)
        return -1;

    if (opt.nflag == false)
        fprintf(builtinFd->out, "\n");
    
    fflush(builtinFd->out);
    return 0;
}