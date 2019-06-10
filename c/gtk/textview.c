#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs glib-2.0 gtk+-2.0 gmodule-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

int
main(int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *text;

  gtk_init(&argc,&argv);
 
  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  text = gtk_text_view_new();
  gtk_container_add (GTK_CONTAINER (window1), text);


  gtk_widget_show_all(window1);
  gtk_widget_set_usize(window1,300,400);
  gtk_main();

  return 0;
}
