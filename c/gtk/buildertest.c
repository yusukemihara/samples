#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs gtkpanda2 gmodule-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtkpanda/gtkpanda.h>

static GObject *label1;

G_MODULE_EXPORT void
on_entry1_activate(GtkEntry *w,gpointer data)
{
	gtk_label_set_text(GTK_LABEL(label1),
		gtk_entry_get_text(GTK_ENTRY(w)));
}

int
main(int argc, char *argv[])
{
	GtkBuilder *builder;
	GObject *window1;
	GObject *entry1;

	gtk_init(&argc,&argv);
	gtk_panda_init(&argc,&argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "buildertest.ui", NULL);
	gtk_builder_connect_signals(builder, NULL);

	window1 = gtk_builder_get_object(builder, "window1");
	label1 = gtk_builder_get_object(builder, "label1");
	entry1 = gtk_builder_get_object(builder, "entry1");
	if (GTK_IS_PANDA_ENTRY(entry1)) {
		fprintf(stderr,"pandaentry\n");
	}

	gtk_widget_show_all(GTK_WIDGET(window1));
	gtk_main();

	return 0;
}
