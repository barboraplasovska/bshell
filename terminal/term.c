#include <vte/vte.h>
#include <err.h>
#include <string.h>


//#define dkRGBA *white;
//#define const gchar *color_white = (gchar *[]){g_strdup("#ffffff"), NULL };
//const gboolean r = gdk_rgba_parse(white,color_white);


static void child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

int options(int argc, char *argv[],char **term,int *shell){
    if (argc == 1 || argc == 2){
        *shell = 0;
        *term = "NaN ~ S4";
        return 1;
    }
    if (strcmp(argv[1],"-bsh") == 0){
        *shell = 1;
        *term = argv[2];
        return 1;
    }
    if (strcmp(argv[1],"-h") == 0||strcmp(argv[1],"--help") == 0){
        printf("NAME: run/n OPTIONS:/n \
                /t By default run without option will lauch default system \
                SHELL with window name: NaN ~ S4/n \
                /t -bsh: use bshell + give window name/n \
                /t -h, --help: print help screen/n");
    }

    return 0;

}
int main(int argc, char *argv[])
{
    /*if (argc > 3)
        err(-1,"Too many arguments./n");
    
    char *term;
    int shell;
    int r = options(argc,argv,&term,&shell);
    if (!r)
        err(-2,"Option error./n");*/

    GtkWidget *window, *terminal;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Nan ~ S4");

    /* Start a new shell */
    gchar **envp = g_get_environ();
    //gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    gchar **command = (gchar *[]){g_strdup("./shell/main"), NULL };
    g_strfreev(envp);
    //vte_terminal_set_color_background(VTE_TERMINAL(terminal),white);
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        NULL,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        0,            /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */ //child_ready
        NULL);        /* user_data */
    
        
    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}
