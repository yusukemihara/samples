#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs gtk+-3.0 glib-2.0 gmodule-2.0 gmodule-export-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static GObject *button1,*button2,*button3,*entry1,*combobox1,*combobox2,*window1;

G_MODULE_EXPORT void
cb_activate(GtkEntry *w,gpointer data)
{
	gchar *text;
	GtkListStore *store;
	GtkTreeIter iter;
	int i;

	text = gtk_entry_get_text(w);
	gtk_button_set_label(GTK_BUTTON(button2),text);
	gtk_button_set_label(GTK_BUTTON(button3),text);

	store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox1)));
	gtk_list_store_clear(store);
	for(i=0;i<10;i++) {
		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,0,text,-1);
	}
}

G_MODULE_EXPORT void
cb_clicked(GtkWidget *w,gpointer data)
{
	gtk_main_quit();
}

int
main(int argc, char *argv[])
{
	GtkBuilder *builder;
	GtkListStore *store;
	GtkCellRendererText *renderer;

	gtk_init(&argc,&argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "fixed.ui", NULL);
	gtk_builder_connect_signals(builder, NULL);

	window1 = gtk_builder_get_object(builder, "window1");
	button1 = gtk_builder_get_object(builder, "button1");
	button2 = gtk_builder_get_object(builder, "button2");
	button3 = gtk_builder_get_object(builder, "button3");
	entry1 = gtk_builder_get_object(builder, "entry1");
	combobox1 = gtk_builder_get_object(builder, "combobox1");
	combobox2 = gtk_builder_get_object(builder, "combobox2");

 	renderer = gtk_cell_renderer_text_new();
	store = gtk_list_store_new(1,G_TYPE_STRING);
	gtk_combo_box_set_model(GTK_COMBO_BOX(combobox1),GTK_TREE_MODEL(store));
	gtk_combo_box_set_model(GTK_COMBO_BOX(combobox2),GTK_TREE_MODEL(store));
	g_object_set(combobox1,"id_column",0,NULL);
	g_object_set(combobox2,"id_column",0,NULL);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox1),GTK_CELL_RENDERER(renderer),TRUE);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox2),GTK_CELL_RENDERER(renderer),TRUE);

	g_signal_connect(entry1,"activate",G_CALLBACK(cb_activate),NULL);
	g_signal_connect(button1,"clicked",G_CALLBACK(cb_clicked),NULL);

	gtk_widget_show_all(GTK_WIDGET(window1));
	gtk_main();

	return 0;
}
