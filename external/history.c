#include "history.h"
#define _GNU_SOURCE

void string_copy(char *from, char *to) {

    while ((*to++ = *from++) != '\0')
        ;
}

void freeNext(struct Node *n)
{
    while (n)
    {
        free(n->array);
        struct Node* next = n->next;
        free(n);
        n = next;
    }
}

void freePrevious(struct Node *n)
{
    while (n)
    {
        free(n->array);
        struct Node* prev = n->prev;
        free(n);
        n = prev;
    }
}

/**
** @brief               goes through the history of the commands and allows to call them
** @param argv          Array of string arguments.
** @param builtinFd     Files.
** @return              Returns 0 in case of no problems.
*/
int history(BuiltinFd *builtinFd)
{
    if(access("myHistory.txt", F_OK) != -1)
    {
        FILE *fp = fopen("myHistory.txt", "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        struct Node* n = NULL;
        struct Node* prev = NULL;

        if (!fp)
        {
            fprintf(builtinFd->err, "history: Failed to access history...check permissions...\n");
        }
        else
        {
            while ((read = getline(&line, &len, fp)) != -1)
            {
                struct Node* current = (struct Node*)malloc(sizeof(struct Node));
                if (n)
                    n->next = current;
                char *content = (char*)malloc(read);
                string_copy(line, content);
		printf("we read: %s", content);
                current->array = content;
                current->prev = prev;
                prev = current;
                n = current;
            }
            char a[2];
            while (true)
            {
                //printf("we put: %i", c);
                printf("%s", n->array);

                char *res = fgets(a, 2, builtinFd->in);
                if (!res)
                    break;

                if (a[0] == 'u')
                {
                    if (n->prev)
                        n = n->prev;
                }
                else if (a[0] == 'd')
                {
                    if (n->next)
                        n = n->next;
                }
                else if (a[0] == '\n')
                {
                    int res = system(n->array);
                    if (res != 0) { } //error
                    break;
                }
                //printf("%i\n", a[0]);

                //fseek(builtinFd->in,0,SEEK_END);

                int ch;
                while((ch = getc(builtinFd->in)) != '\n' && ch != EOF);
            }
        }

        freeNext(n->next);
        freePrevious(n->prev);
        free(n->array);
        free(n);

        fclose(fp);
    }
    else
    {
        fprintf(builtinFd->out, "There is no history...\n");
    }

    return 0;
}

int main(int argc, char **argv)
{
    struct builtinFd *terminal = NULL;
    terminal = (struct builtinFd *) malloc(sizeof(struct builtinFd));
    terminal->in = stdin;//STDIN_FILENO;
    terminal->out = stdout;//(FILE *) STDOUT_FILENO;
    terminal->err = stderr;//(FILE *) STDERR_FILENO;
    terminal->inNo =  STDIN_FILENO;
    terminal->outNo = STDOUT_FILENO;
    terminal->errNo = STDOUT_FILENO;

    if (argc){}
    if (argv[0] == NULL)
    {
        history(terminal);
    }
    else
    {
        fprintf(terminal->err ,"History doesn't take any arguments\n");
        return -1;
    }

    free(terminal);
    return 0;
}
