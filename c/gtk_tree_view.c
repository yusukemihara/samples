/*
 *   gcc -g `pkg-config --libs --cflags gtk+-2.0` gtk_tree_view.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <glib.h>
#include <gtk/gtk.h>

struct DicDialog {
  GtkWidget *dialog;
  GtkWidget *treeview;
  GtkWidget *frame;
  GtkWidget *name_entry;
  GtkWidget *occurs_spin;
  GtkWidget *length_hbox;
  GtkWidget *length_spin;
};

static struct DicDialog *dic_dialog = NULL;

#if 1
  #define _(a) ((gchar*)a)
#endif

enum
{
  COLUMN_ICON = 0,
  COLUMN_TREE,
  COLUMN_OCCURS,
  COLUMN_USED,
  NUM_COLS
} ;

enum
{
  ITEM_TYPE_NODE = 0,
  ITEM_TYPE_STRING,
  ITEM_TYPE_IMAGE
};

#define ICON_NODE GTK_STOCK_DIRECTORY
#define ICON_STRING GTK_STOCK_BOLD
#define ICON_IMAGE GTK_STOCK_CONVERT

static int
get_node_item_type(gchar *stock)
{
  if (!strcmp(stock,ICON_NODE)) {
    return ITEM_TYPE_NODE;
  } else if (!strcmp(stock,ICON_STRING)) {
    return ITEM_TYPE_STRING;
  } else if (!strcmp(stock,ICON_IMAGE)) {
    return ITEM_TYPE_IMAGE;
  }
  return ITEM_TYPE_NODE;
}

static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore *treestore;
  GtkTreeIter top, child;

  treestore = gtk_tree_store_new(NUM_COLS,
                  G_TYPE_STRING, G_TYPE_STRING,G_TYPE_INT,G_TYPE_STRING);

  gtk_tree_store_append(treestore, &top, NULL);
  gtk_tree_store_set(treestore, &top,
                     COLUMN_ICON, ICON_STRING,
                     COLUMN_TREE, "STRING1",
                     COLUMN_OCCURS, 1,
                     COLUMN_USED, "-",
                     -1);
  gtk_tree_store_append(treestore, &top, NULL);
  gtk_tree_store_set(treestore, &top,
                     COLUMN_ICON, ICON_NODE,
                     COLUMN_TREE, "NODE1",
                     COLUMN_OCCURS, 10,
                     COLUMN_USED, "-",
                     -1);
  gtk_tree_store_append(treestore, &child, &top);
  gtk_tree_store_set(treestore, &child,
                     COLUMN_ICON, ICON_STRING,
                     COLUMN_TREE, "STRING2",
                     COLUMN_OCCURS, 10,
                     COLUMN_USED, "0/10",
                     -1);
  gtk_tree_store_append(treestore, &child, &top);
  gtk_tree_store_set(treestore, &child,
                     COLUMN_ICON, ICON_IMAGE,
                     COLUMN_TREE, "IMAGE1",
                     COLUMN_OCCURS, 10,
                     COLUMN_USED, "0/10",
                     -1);

  return GTK_TREE_MODEL(treestore);
}

static gboolean
cb_drag_drop(GtkWidget *widget,
  GdkDragContext *drag_context,
  gint x,
  gint y,
  guint time,
  gpointer data)
{
  GtkTreeModel *model;
  GtkTreePath *dest;
  GtkTreeIter iter;
  GtkTreeViewDropPosition pos;
  gchar *value;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));
  if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
        x,y,&dest,&pos)) {
    if (pos == GTK_TREE_VIEW_DROP_INTO_OR_BEFORE ||
        pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER) {
      if (gtk_tree_model_get_iter(model,&iter,dest)) {
        gtk_tree_model_get(model, &iter, COLUMN_ICON, &value,-1);
        if (value != NULL && get_node_item_type(value) != ITEM_TYPE_NODE) {
          g_free(value);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static gboolean
cb_drag_data_received(GtkWidget *widget,
  GdkDragContext *context,
  gint x,
  gint y,
  GtkSelectionData *data,
  guint info,
  guint time,
  gpointer user_data)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreePath *dest_path;
  GtkTreeIter src_iter,dest_iter,child_iter;
  GtkTreeViewDropPosition pos;
  gchar *icon,*tree,*used;
  int occurs;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  if (gtk_tree_selection_get_selected(selection, &model, &src_iter)) {
    if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
          x,y,&dest_path,&pos)) {
      gtk_tree_model_get_iter(model,&dest_iter,dest_path);
      gtk_tree_model_get(model, &src_iter,
        COLUMN_ICON, &icon,
        COLUMN_TREE, &tree,
        COLUMN_OCCURS, &occurs,
        COLUMN_USED, &used,
        -1);
      switch(pos) {
      case GTK_TREE_VIEW_DROP_BEFORE:
        gtk_tree_store_insert_before(GTK_TREE_STORE(model),&child_iter,NULL,&dest_iter);
        break;
      case GTK_TREE_VIEW_DROP_AFTER:
        gtk_tree_store_insert_after(GTK_TREE_STORE(model),&child_iter,NULL,&dest_iter);
        break;
      case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
      case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
        gtk_tree_store_append(GTK_TREE_STORE(model),&child_iter,&dest_iter);
        break;
      }
      gtk_tree_store_set(GTK_TREE_STORE(model), &child_iter,
        COLUMN_ICON, icon,
        COLUMN_TREE, tree,
        COLUMN_OCCURS, occurs,
        COLUMN_USED, used,
        -1);
      gtk_tree_store_remove(GTK_TREE_STORE(model),&src_iter);
    }
  }
  return FALSE;
}

static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkCellRenderer *renderer2;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();
  g_object_set(G_OBJECT(view),"enable-tree-lines",TRUE,NULL);
#if 0
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(view),TRUE);
#else
  const GtkTargetEntry row_targets[] = {
    { "SAMPLE", GTK_TARGET_SAME_APP, 0 }
  };
  gtk_tree_view_enable_model_drag_source(GTK_TREE_VIEW(view),
    GDK_BUTTON1_MASK,
    row_targets,
    G_N_ELEMENTS (row_targets),
    GDK_ACTION_MOVE);
  gtk_tree_view_enable_model_drag_dest(GTK_TREE_VIEW(view),
    row_targets,
    G_N_ELEMENTS (row_targets),
    GDK_ACTION_MOVE);
#endif
  g_signal_connect(G_OBJECT(view),"drag-drop",
    G_CALLBACK(cb_drag_drop),NULL);
  g_signal_connect(G_OBJECT(view),"drag-data-received",
    G_CALLBACK(cb_drag_data_received),NULL);

  /* tree */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "TREE");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_pixbuf_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
    "stock-id",COLUMN_ICON);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
    "text", COLUMN_TREE);

  /* occurs */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "OCCURS");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
    "text", COLUMN_OCCURS);

  /* used */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "USED");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
    "text", COLUMN_USED);


  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  return view;
}

static void
cb_delete_button(GtkToolButton *button,
  GtkTreeSelection *select)
{
  GtkTreeIter iter,new;
  GtkTreeModel *model;
  GtkTreeStore *store;
  gchar *stock;

  if (gtk_tree_selection_get_selected(
      select, &model, &iter)) {
    store = GTK_TREE_STORE(model);
    gtk_tree_store_remove(store,&iter);
  }
}

static void
cb_add_button(GtkToolButton *button,
  GtkTreeSelection *select)
{
  GtkTreeIter iter,new;
  GtkTreeModel *model;
  GtkTreeStore *store;
  gchar *stock;
  gchar *parent_type;

  if (gtk_tree_selection_get_selected(
      select, &model, &iter)) {
    gtk_tree_model_get(model, &iter, COLUMN_ICON, &parent_type,  -1);
    store = GTK_TREE_STORE(model);
    if (get_node_item_type(parent_type) == ITEM_TYPE_NODE) {
      gtk_tree_store_insert_after(store, &new, &iter, NULL);
    } else {
      gtk_tree_store_insert_after(store, &new, NULL, &iter);
    }
    g_free(parent_type);
  } else {
    store = GTK_TREE_STORE(model);
    gtk_tree_store_insert_after(store, &new, NULL, NULL);
  }

  stock = (gchar*)gtk_tool_button_get_stock_id(button);

  switch(get_node_item_type(stock)) {
  case ITEM_TYPE_NODE:
    gtk_tree_store_set(store, &new,
      COLUMN_ICON, ICON_NODE,
      COLUMN_TREE, "NODE",
      COLUMN_OCCURS, 10,
      COLUMN_USED, "-",
      -1);
    break;
  case ITEM_TYPE_STRING:
    gtk_tree_store_set(store, &new,
      COLUMN_ICON, ICON_STRING,
      COLUMN_TREE, "STRING",
      COLUMN_OCCURS, 10,
      COLUMN_USED, "-",
      -1);
    break;
  case ITEM_TYPE_IMAGE:
    gtk_tree_store_set(store, &new,
      COLUMN_ICON, ICON_IMAGE,
      COLUMN_TREE, "IMAGE",
      COLUMN_OCCURS, 10,
      COLUMN_USED, "-",
      -1);
    break;
  }
}

static GtkWidget*
create_toolbar(GtkTreeSelection *select)
{
  GtkWidget *toolbar;
  GtkToolItem *node_button;
  GtkToolItem *str_button;
  GtkToolItem *img_button;
  GtkToolItem *delete_button;

  toolbar = gtk_toolbar_new();

  node_button = gtk_tool_button_new_from_stock(ICON_NODE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar),GTK_TOOL_ITEM(node_button),0);
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(node_button),"ADD NODE");
  g_signal_connect(G_OBJECT(node_button),"clicked",
    G_CALLBACK(cb_add_button),select);

  str_button = gtk_tool_button_new_from_stock(ICON_STRING);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar),GTK_TOOL_ITEM(str_button),1);
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(str_button),"ADD STRING");
  g_signal_connect(G_OBJECT(str_button),"clicked",
    G_CALLBACK(cb_add_button),select);

  img_button = gtk_tool_button_new_from_stock(ICON_IMAGE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar),GTK_TOOL_ITEM(img_button),2);
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(img_button),"ADD IMAGE");
  g_signal_connect(G_OBJECT(img_button),"clicked",
    G_CALLBACK(cb_add_button),select);

  delete_button = gtk_tool_button_new_from_stock(GTK_STOCK_DELETE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar),GTK_TOOL_ITEM(delete_button),3);
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(delete_button),"DELETE ITEM");
  g_signal_connect(G_OBJECT(delete_button),"clicked",
    G_CALLBACK(cb_delete_button),select);

  return toolbar;
}

static void
cb_selection_changed(GtkTreeSelection *sel,
  gpointer data)
{
  struct DicDialog *dic_dialog;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *icon;
  gchar *tree;
  int occurs;
  gchar *length;

  if (gtk_tree_selection_get_selected(sel,&model,&iter)) {
    dic_dialog = (struct DicDialog*)data;
    gtk_tree_model_get(model, &iter, COLUMN_ICON, &icon,  -1);
    gtk_tree_model_get(model, &iter, COLUMN_TREE, &tree,  -1);
    gtk_tree_model_get(model, &iter, COLUMN_OCCURS, &occurs,  -1);
#if 0
    gtk_tree_model_get(model, &iter, COLUMN_LENGTH, &length,  -1);
#else
    length = g_strdup("10");
#endif

    gtk_entry_set_text(GTK_ENTRY(dic_dialog->name_entry),tree);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(dic_dialog->occurs_spin),
      (gdouble)occurs);

    if (length != NULL) {
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(dic_dialog->length_spin),
        (gdouble)atof(length));
    }

    switch(get_node_item_type(icon)) {
    case ITEM_TYPE_NODE:
      gtk_frame_set_label(GTK_FRAME(dic_dialog->frame),_("Node"));
      gtk_widget_hide(dic_dialog->length_hbox);
      break;
    case ITEM_TYPE_STRING:
      gtk_frame_set_label(GTK_FRAME(dic_dialog->frame),_("String"));
      gtk_widget_show(dic_dialog->length_hbox);
      break;
    case ITEM_TYPE_IMAGE:
      gtk_frame_set_label(GTK_FRAME(dic_dialog->frame),_("Image"));
      gtk_widget_hide(dic_dialog->length_hbox);
      break;
    }   
    
    g_free(icon);
    g_free(tree);
    g_free(length);
  }
}

void create_dic_dialog()
{
  GtkWidget *dialog, *vbox;
  GtkWidget *hbox, *label, *combo, *separator;
  GtkWidget *toolbar, *treeview;
  GtkTreeSelection *selection; 
  GtkWidget *frame,*vbox2;
  GtkWidget *name_entry,*occurs_spin,*length_spin;
  GtkWidget *change_button;

  dic_dialog = g_new0(struct DicDialog,1);

  /* dialog */
  dialog = gtk_dialog_new();
  dic_dialog->dialog = dialog;
  gtk_window_set_title(GTK_WINDOW(dialog), _("Dictionary"));
  gtk_window_set_role(GTK_WINDOW(dialog), "dictionary_window");
  gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_default_size(GTK_WINDOW(dialog),250,600);

  vbox = GTK_DIALOG(dialog)->vbox;
  
  /* diagram list */
  hbox = gtk_hbox_new(FALSE,1);
  label = gtk_label_new(_("Diagram:"));
  gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,2);

  combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"diagram1");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"diagram2");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"diagram3");
  gtk_box_pack_start(GTK_BOX(hbox),combo,TRUE,TRUE,2);

  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 2);

  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 0);

  /* treeview */
  treeview = create_view_and_model();
  dic_dialog->treeview = treeview;
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(G_OBJECT(selection),"changed",
    G_CALLBACK(cb_selection_changed),dic_dialog);

  /* toolbar */
  toolbar = create_toolbar(selection);

  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 1);

  /* frame */
  frame = gtk_frame_new("");
  dic_dialog->frame = frame;
  vbox2 = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(frame),vbox2);

  hbox = gtk_hbox_new(FALSE,0);
  label = gtk_label_new(_("Name"));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
  name_entry = gtk_entry_new();
  dic_dialog->name_entry = name_entry;
  gtk_box_pack_start(GTK_BOX(hbox),name_entry,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);

  hbox = gtk_hbox_new(FALSE,0);
  label = gtk_label_new(_("Occurs"));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
  occurs_spin = gtk_spin_button_new_with_range(1,1000,1);
  dic_dialog->occurs_spin = occurs_spin;
  gtk_box_pack_start(GTK_BOX(hbox),occurs_spin,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);

  dic_dialog->length_hbox = hbox = gtk_hbox_new(FALSE,0);
  label = gtk_label_new(_("Length"));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,FALSE,0);
  length_spin = gtk_spin_button_new_with_range(1,1000,1);
  dic_dialog->length_spin = length_spin;
  gtk_box_pack_start(GTK_BOX(hbox),length_spin,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
  
  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox2), separator, FALSE, FALSE, 0);

  hbox = gtk_hbox_new(FALSE,0);
  change_button = gtk_button_new_with_label(_("Change"));
  gtk_box_pack_end(GTK_BOX(vbox2), change_button, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, TRUE, 1);

  gtk_widget_show_all(dialog);
}

int
main (int argc, char **argv)
{
  gtk_init(&argc, &argv);

  create_dic_dialog();

  gtk_main();

  return 0;
}

