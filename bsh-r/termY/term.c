#include <vte/vte.h>

//GdkRGBA *white; 
//gboolean r = gdk_rgba_parse(white,"#ffff"); 

static void child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

int main(int argc, char *argv[])
{
    if (argc > 2)
        return 0;
    GtkWidget *window, *terminal;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "NaN ~ S4");

    /* Start a new shell */
    gchar **envp = g_get_environ();
    //gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    gchar **command = (gchar *[]){g_strdup("./shell-charbel/main"), NULL };
    g_strfreev(envp);
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
        child_ready,  /* callback */
        NULL);        /* user_data */
    
    if (strcmp(argv[1],"white")){
        
        vte_terminal_set_colors(VTE_TERMINAL(terminal),NULL,NULL,NULL,0);
    }
    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}
