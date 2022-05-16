#include "createAnimation.h"

void _createAnimation(char* path, size_t pathLength, BuiltinFd *builtinFd)
{
    mkdir(path, 0700);
    char ch;
    size_t frameNumber = 1;
    do
    {
        fprintf(builtinFd->out, "Enter the animation for your frame:\n");
        char str[MAX_LIMIT];
        fgets(str, MAX_LIMIT, builtinFd->in);

        //create textfile here
        path = realloc(path, pathLength + 2);
        path[pathLength - 1] = '/';
        path[pathLength] = frameNumber + '0';
        path[pathLength + 1] = '\0';
        FILE *fp = fopen(path, "a");

        //add contents to textfile
        fprintf(fp, "%s", str);

        ch = ' ';
        while (ch != 'y' && ch != 'n')
        {
            fprintf(builtinFd-> out, "Would you like to add another frame? (y/n)\n");
            ch = getc(builtinFd->in);

            int cha;
            while((cha = getc(builtinFd->in)) != '\n' && cha != EOF);
        }
        frameNumber++;
    } while (ch == 'y');
}

/**
** @brief               Create an animation.
** @param name          Animation name.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int createAnimation(char* name, BuiltinFd *builtinFd)
{
    struct stat st = {0};

    size_t pathLength = 11 + strlen(name) + 1;
    char *path = malloc(pathLength);
    strncpy(path, "Animations/", 12);

    for (size_t i = 11; i < pathLength - 1; i++)
    {
        path[i] = name[i - 11];
    }
    path[pathLength - 1] = '\0';

    if (stat(path, &st) == -1)
    {
        _createAnimation(path, pathLength, builtinFd);
    }
    else
    {
        char ch = ' ';
        while (ch != 'y' && ch != 'n')
        {
            fprintf(builtinFd->out, "%s already exists, would you like you replace it? (y/n)\n", path);
            ch = getc(builtinFd->in);
            int cha;
            while((cha = getc(builtinFd->in)) != '\n' && cha != EOF);
        }
        if (ch == 'y')
        {
            // call remove animation
            size_t commandLength = 23 + strlen(name) + 1;
            char* command = malloc(commandLength);
            strncpy(command, "./test removeanimation ", 24);

            for (size_t i = 23; i < commandLength - 1; i++)
            {
                command[i] = name[i - 23];
            }
            command[commandLength - 1] = '\0';

            system(command);

            _createAnimation(path, pathLength, builtinFd);
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc){}
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin; //STDIN_FILENO;
    terminal->out = stdout; //(FILE *) STDOUT_FILENO;
    terminal->err = stderr; //(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;
    AppendToHistory(argv, "createanimation", terminal);

    if (argv[1] != NULL)
    {
        fprintf(terminal->err, "createanimation: This function only takes 1 parameter\n");
        return -1;
    }
    
    createAnimation(argv[0],terminal);

    free(terminal);

    return 0;
}
