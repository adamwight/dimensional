/* This is a very simple program to create the mandelbrot set */

#include <math.h>
#include <gtk/gtk.h>

class mandle
{
public:
    mandle(GtkWidget* window)
    {
        gtk_widget_set_size_request (window, 700, 700);

        g_signal_connect(G_OBJECT(window), "expose_event",
            G_CALLBACK(expose_cb), NULL);
        gtk_widget_show_all(window);
    }

    static gboolean expose_cb(GtkWidget *widget, GdkEventExpose *event, gpointer data)
    {
      mandle_window(widget, -2, 2, -2, 2,
                    widget->allocation.width, widget->allocation.height);
     
      return TRUE;
    }

    static void mandle_window(
        GtkWidget* widget,
        double xstart, double xend,
        double ystart, double yend,
        int width, int height)
    {
      double x,y;
      double xstep, ystep;
      double z,zi,newz,newzi;
      int iter = 200;
      int i,j,k, color;
      int inset;

      GdkDrawable* window = widget->window;
      GdkGC* gc = widget->style->fg_gc[GTK_WIDGET_STATE (widget)];

      /* these are used for calculating the points corresponding to the pixels */
      xstep = (xend-xstart)/width;
      ystep = (yend-ystart)/height;

      /*the main loop */
      x = xstart;
      y = ystart;
      for (i=0; i<height; i++)
      {
        for (j=0; j<width; j++)
        {
          z = 0;
          zi = 0;
          inset = 1;
          for (k=0; k<iter; k++)
          {
            /* z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2 */
        newz = (z*z)-(zi*zi) + x;
        newzi = 2*z*zi + y;
            z = newz;
            zi = newzi;
        if(((z*z)+(zi*zi)) > 4)
        {
          inset = 0;
          color = k;
          k = iter;
        }
          }
          if (inset)
          {
    //black
            gdk_draw_point(window, gc, i, j);
          }
          else
          { 
            GdkColor c;
            c.red = color * (65535 / iter);
            c.green = color * (65535 / iter) / 2;
            c.blue = color * (65535 / iter) / 2;
            gdk_gc_set_rgb_fg_color(gc, &c);
            gdk_draw_point(window, gc, i, j);
          }
          x += xstep;
        }
        y += ystep;
        x = xstart;
      }
    } 
};
