#include <stdio.h>
#include <gtk/gtk.h>

#include "ui_builder.h"
#include "main_window.h"
#include "mand.h"
#include "koch_window.h"

const char* GTKCONFIG = "dimensional-gtk.xml";

int main(int argc, char **argv)
{
    gtk_set_locale ();
    gtk_init (&argc, &argv);

    ui_builder* ui = new ui_builder(GTKCONFIG);
    new mandle(ui->get("mandle"));
    new koch_window(ui->get("koch"),
        ui->get("iterEntry"),
        ui->action("save"));
    new main_window(ui->get("main"));

    gtk_main ();

    return 0;
}
