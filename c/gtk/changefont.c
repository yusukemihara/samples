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

static void
cb_font_set(
  GtkFontButton *fb,
  GtkStyle *style)
{
  gchar *rcstr;

  rcstr = g_strdup_printf(
    "style \"glclient2\" {"
    "  font_name=\"%s\""
    "}"
    "widget_class \"*.*\" style \"glclient2\""
    ,gtk_font_button_get_font_name(fb));
  gtk_rc_parse_string(rcstr);
  gtk_rc_reset_styles(gtk_settings_get_default());
  g_free(rcstr);
}

int
main(int argc, char *argv[])
{
  GtkWidget *window1;
  GtkWidget *table1;
  GtkWidget *label1;
  GtkWidget *entry1;
  GtkWidget *button1;
  GtkWidget *font_button1;

  gtk_init(&argc,&argv);
 
  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");

  table1 = gtk_table_new (2, 2, FALSE);

  gtk_container_add (GTK_CONTAINER (window1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 4);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 4);

  label1 = gtk_label_new ("ラベル");
  gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  entry1 = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  button1 = gtk_button_new_with_label("ボタン");
  gtk_table_attach (GTK_TABLE (table1), button1, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  font_button1 = gtk_font_button_new();
  gtk_table_attach (GTK_TABLE (table1), font_button1, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  g_signal_connect(G_OBJECT(font_button1),"font-set",
    G_CALLBACK(cb_font_set),NULL);

  gtk_widget_show_all(window1);
  gtk_main();

  return 0;
}
