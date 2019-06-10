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
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

static void
cb_clicked(
  GtkButton *b,
  gpointer data)
{
  fprintf(stderr,"clicked %s\n",gtk_button_get_label(b));
}

int
main(int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *hbox1;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkAccelGroup *ag;

  gtk_init(&argc,&argv);
 
  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  hbox1 = gtk_hbox_new(FALSE,1);
  button1 = gtk_button_new_with_label("F10");
  button2 = gtk_button_new_with_label("Shift-F10");

  gtk_box_pack_start(GTK_BOX(hbox1),button1,TRUE,TRUE,1);
  gtk_box_pack_start(GTK_BOX(hbox1),button2,TRUE,TRUE,1);

  g_signal_connect(G_OBJECT(button1),"clicked",
    G_CALLBACK(cb_clicked),NULL);
  g_signal_connect(G_OBJECT(button2),"clicked",
    G_CALLBACK(cb_clicked),NULL);

  gtk_container_add(GTK_CONTAINER(window1),hbox1);

  ag = gtk_accel_group_new();
  gtk_widget_add_accelerator(button1,"clicked",ag,
    GDK_F10,0,GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator(button2,"clicked",ag,
    GDK_F10,GDK_SHIFT_MASK,GTK_ACCEL_VISIBLE);

  gtk_window_add_accel_group(GTK_WINDOW(window1),ag);

  gtk_widget_show_all(window1);
  gtk_main();

  return 0;
}
