#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed -lm `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>

G_MODULE_EXPORT void
cb_clicked(GtkButton *button,
  gpointer data)
{
  const gchar *label;
  static GdkColor color;
  GdkColormap *colormap;
  GtkColorButton *colorbutton;

  colorbutton = GTK_COLOR_BUTTON(data);
  colormap = gdk_colormap_get_system();
  gdk_colormap_alloc_color(colormap,&color,TRUE,FALSE);
  label = gtk_button_get_label(button);
  if (!strcmp("red",label)) {
    gdk_color_parse("#ff0000",&color);
  } else if (!strcmp("blue",label)) {
    gdk_color_parse("#0000ff",&color);
  } else if (!strcmp("green",label)) {
    gdk_color_parse("#00ff00",&color);
  } else if (!strcmp("yellow",label)) {
    gdk_color_parse("#ffff00",&color);
  } else {
    gdk_color_parse("#00ffff",&color);
  }
  gtk_color_button_set_color(colorbutton,&color);
}

static double 
scale_round (double val, double factor) 
{ 
  val = floor (val * factor + 0.5); 
  val = MAX (val, 0); 
  val = MIN (val, factor); 
  return val; 
}

G_MODULE_EXPORT void
cb_color_set(GtkColorButton *colorbutton,
  gpointer data)
{
  GdkColor color;

  gtk_color_button_get_color(colorbutton,&color);
  fprintf(stderr,"%s\n",gdk_color_to_string(&color));
  fprintf(stderr,"[#%02X%02X%02X]\n",
    (guint)(color.red/256.0),
    (guint)(color.green/256.0),
    (guint)(color.blue/256.0)
  );
}

int
main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *button1,*button2,*button3;
  GtkWidget *colorbutton;
  GtkWidget *vbox;

  gtk_set_locale();
  gtk_init(&argc,&argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vbox = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(window),vbox);

  colorbutton = gtk_color_button_new();
  gtk_color_button_set_title(GTK_COLOR_BUTTON(colorbutton),"Color Button");
  g_signal_connect(G_OBJECT(colorbutton),"color-set",
    G_CALLBACK(cb_color_set),NULL);
  gtk_box_pack_start(GTK_BOX(vbox),colorbutton,TRUE,TRUE,0);

  button1 = gtk_button_new_with_label("red");
  g_signal_connect(G_OBJECT(button1),"clicked",
    G_CALLBACK(cb_clicked),colorbutton);
  gtk_box_pack_start(GTK_BOX(vbox),button1,TRUE,TRUE,0);

  button2 = gtk_button_new_with_label("blue");
  g_signal_connect(G_OBJECT(button2),"clicked",
    G_CALLBACK(cb_clicked),colorbutton);
  gtk_box_pack_start(GTK_BOX(vbox),button2,TRUE,TRUE,0);

  button3 = gtk_button_new_with_label("green");
  g_signal_connect(G_OBJECT(button3),"clicked",
    G_CALLBACK(cb_clicked),colorbutton);
  gtk_box_pack_start(GTK_BOX(vbox),button3,TRUE,TRUE,0);

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();
  
  return 0;
}
