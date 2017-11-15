class main_window
{
public:
    main_window(GtkWidget* window)
    {
        //g_signal_connect(G_OBJECT(window), "expose_event",
            //G_CALLBACK(expose_cb), NULL);
        gtk_widget_show_all(window);
    }
};
