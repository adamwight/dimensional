#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

#include "koch.h"
#include "draw_gdk.h"
#include "draw_cairo.h"
#include "thread_busy.h"

const int save_target_segs = 128 * 1024;

int last_nsegs = 0;
double last_min_length;

class koch_window : public busy_threaded
{
    GtkWidget* window;

    generator g;
    int cur_handle;

public:
    koch_window(GtkWidget* w, GtkWidget* iterEntry, GtkAction* save)
        : g(4), window(w)
    {
        cur_handle = -1;
        gtk_widget_set_size_request (window, 700, 700);

        gtk_widget_add_events(window, GDK_BUTTON_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
        g_signal_connect(G_OBJECT(window), "expose_event",
            G_CALLBACK(expose_cb), this);
        g_signal_connect(G_OBJECT(window), "button_press_event",
            G_CALLBACK(begin_drag_cb), this);
        g_signal_connect(G_OBJECT(window), "motion_notify_event",
            G_CALLBACK(drag_motion_cb), this);
        g_signal_connect(G_OBJECT(window), "button_release_event",
            G_CALLBACK(end_drag_cb), this);

        g_signal_connect(G_OBJECT(save), "activate",
            G_CALLBACK(save_cb), this);

        // Koch curve
        g[0] = segment(0,0);
        g[1] = segment(1/3.0,0);
        g[2] = segment(1/3.0 + cos(pi/3) / 3, sin(pi/3) / 3);
        g[3] = segment(2/3.0, 0);
        g[4] = segment(1, 0, END);

        //segments = 3;
        //generator = new cartesian[segments + 1];
        //deltas = new polar[segments];
        //generator[0] = cartesian(0,0);
        //generator[1] = cartesian(-1/3.0, 2/3.0);
        //generator[2] = cartesian(1/3.0, 4/3.0);
        //generator[3] = cartesian(1,1);

        //g_signal_connect(G_OBJECT(iterEntry), "
        gtk_widget_show_all(window);
    }

    static void save_cb(GtkMenuItem* item, gpointer data)
    {
        koch_window* koch = (koch_window*)data;
        koch->thread_spawn(false, save_task, koch);
    }

    static void* save_task(gpointer data)
    {
        printf("saving...\nname: ");
        char name[100];
        fgets(name, 100, stdin);
        name[strlen(name) - 1] = 0; // CR
        koch_window* koch = (koch_window*)data;
        char filename[100];
        snprintf(filename, 100, "%s.ps", name);
        struct stat fs;
        if (0 == stat(filename, &fs)) {
            fprintf(stderr, "not overwriting your file.\n");
            return NULL;
        }
        draw_cairo ps(filename);
    //
    //TODO no reason to lock out multiple save tasks now

        // Try to increase resolution (finest length of fractal leg)
        // by waving hands over experimental data for this generator
        if (debug > 0)
            printf("lasts: length %f segs=%d\n", last_min_length, last_nsegs);
        double rez = last_min_length /
            pow((save_target_segs / last_nsegs), 1.0 / 2);
        //double rez = last_min_length / (save_target_segs / last_nsegs);
        printf("adjusting resolution by a factor of %f for vector save\n",
            rez / last_min_length);
        koch_curve c(&ps, koch->g, rez);
        c.generate();
        ps.blit();

        char cmd[256];
        snprintf(cmd, 256, "ps2pdf %s", filename);
        system(cmd);
        printf("...done with save\n");
    }

    static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
    {
        koch_window* koch = (koch_window*)data;
        koch->draw(true);
     
        return TRUE;
    }

    void draw(bool insist)
    {
        draw_task(this);
        //thread_spawn(insist, draw_task, this);
    }

    static void* draw_task(void* arg)
    {
        ((koch_window *)arg)->draw_curve();
    }

    void draw_curve() {
        GdkDrawable* w = window->window;
        GdkGC* gc = window->style->fg_gc[GTK_WIDGET_STATE (window)];
        double rez = .01;
        koch_curve c(new draw_gdk(w, gc), g, rez);
            if (g.segments > 10)
                printf("** bad segs!");
        c.refresh();

        last_min_length = rez;
        last_nsegs = c.nseg;
    }

    static gboolean begin_drag_cb(GtkWidget *widget, GdkEventButton *event, gpointer data)
    {
      koch_window* koch = (koch_window*)data;
      //TODO or closer to a line?
      koch->cur_handle = koch->g.find_handle((int)event->x, (int)event->y);

      if (koch->cur_handle > -1) {
          // cut at a vertex and create a gap
          if (event->button == 3) {
              cartesian loc = screen::unmap((int)event->x, (int)event->y);
              segment new_handle(loc.x, loc.y, GAP);
              koch->g.insert(koch->cur_handle, new_handle);
              //koch->cur_handle++; // XXX?
          }

          return TRUE;
      }
      return FALSE;
    }

    static gboolean end_drag_cb(GtkWidget *widget, GdkEventButton *event, gpointer data)
    {
      koch_window* koch = (koch_window*)data;
      if (koch->cur_handle > -1) {
          //koch->curve->refresh();
          koch->draw(true);
          koch->cur_handle = -1;
          return TRUE;
      }
      return FALSE;
    }

    static gboolean drag_motion_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data)
    {
      koch_window* koch = (koch_window*)data;
      int index = koch->cur_handle;
      if (index > -1) {
          cartesian loc = screen::unmap((int)event->x, (int)event->y);
          koch->g[index] = segment(loc.x, loc.y, koch->g[index].link);
          if (!gtk_events_pending())
              koch->draw(true);
          return TRUE;
      }
      return FALSE;
    }
};
