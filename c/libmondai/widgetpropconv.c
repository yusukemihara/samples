#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libmondai libglade-panda-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmondai.h>
#include <RecParser.h>
#include <gtk/gtk.h>
#include <gtkpanda/gtkpanda.h>
#include <glade/glade.h>

struct RecordTree {
  char *name;
  char *longname;
  int type;
  GList *children;
  int charsize;
  int arraysize;
  int len;
  int slen;
  struct RecordTree *parent;
};

struct RecordTree*
MakeRecordTree(
  char *name,
  char *longname,
  ValueStruct *val)
{
  char a_name[1024],a_longname[1024];
  struct RecordTree *rec,*child;
  ValueStruct *cval;
  int i;

  rec = g_new0(struct RecordTree,1);
  rec->name = g_strdup(name);
  rec->longname = g_strdup(longname);
  rec->type = val->type;
  rec->children = NULL;
  rec->charsize = 0;
  rec->arraysize = 0;
  rec->len = 0;
  rec->slen = 0;
  rec->parent = NULL;

  if (IS_VALUE_ARRAY(val)) {
    sprintf(a_name,"%s",rec->name,0);
    sprintf(a_longname,"%s[%d]",rec->longname,0);
    rec->arraysize = ValueArraySize(val);
    cval = ValueArrayItem(val,0);
    child = MakeRecordTree(a_name,a_longname,cval);
    child->parent = rec;
    rec->children = g_list_append(rec->children,child);
  } else if (IS_VALUE_RECORD(val)) {
    for(i=0;i<ValueRecordSize(val);i++) {
      sprintf(a_longname,"%s.%s",rec->longname,ValueRecordName(val,i));
      cval = ValueRecordItem(val,i);
      child = MakeRecordTree(ValueRecordName(val,i),a_longname,cval);
      rec->children = g_list_append(rec->children,child);
      child->parent = rec;
    }
  } else if (IS_VALUE_STRING(val)) {
    rec->charsize = ValueStringLength(val);
  } else if (val->type == GL_TYPE_NUMBER) {
    rec->len = ValueFixedLength(val);
    rec->slen = ValueFixedSlen(val);
  }

  return rec;
}

const char*
GetRecordType(
  int type)
{
  switch(type) {
  case GL_TYPE_BOOL:
    return "bool";
  case GL_TYPE_BYTE:
    return "byte";
  case GL_TYPE_BINARY:
    return "binary";
  case GL_TYPE_CHAR:
    return "char";
  case GL_TYPE_VARCHAR:
    return "varchar";
  case GL_TYPE_FLOAT:
    return "float";
  case GL_TYPE_TIMESTAMP:
    return "timestamp";
  case GL_TYPE_DATE:
    return "date";
  case GL_TYPE_TIME:
    return "time";
  case GL_TYPE_INT:
    return "int";
  case GL_TYPE_NUMBER:
    return "number";
  case GL_TYPE_TEXT:
    return "text";
  case GL_TYPE_OBJECT:
    return "object";
  case GL_TYPE_DBCODE:
    return "dbcode";
  }
  return "";
}

void
PrintLevel(int l)
{
  int i;
  for(i=0;i<l;i++) {
    printf("  ");
  }
}

void
OutputRecord(
	struct RecordTree* rec,
    int level)
{
  GList *list;

  if (rec->type == GL_TYPE_RECORD) {
    PrintLevel(level);
    printf("%s {\n",rec->name);
    for(list = rec->children; list != NULL; list = list->next) {
      OutputRecord((struct RecordTree*)list->data,level+1);
    }
    if (rec->parent != NULL && rec->parent->type == GL_TYPE_ARRAY) {
      PrintLevel(level);
      printf("}");
    } else {
      PrintLevel(level);
      printf("};\n");
    }
  } else if (rec->type == GL_TYPE_ARRAY) {
    for(list = rec->children; list != NULL; list = list->next) {
      OutputRecord((struct RecordTree*)list->data,level+1);
    }
    printf("[%d];\n",rec->arraysize);
  } else {
    PrintLevel(level);
    printf("%s %s",rec->name,GetRecordType(rec->type));
    if (IS_VALUE_STRING(rec)) {
      printf("(%d)",rec->charsize);
    }
    if (rec->type == GL_TYPE_NUMBER) {
      if (rec->slen == 0) {
        printf("(%d)",rec->len);
      } else {
        printf("(%d,%d)",rec->len,rec->slen);
      }
    }
    if (rec->parent != NULL && rec->parent->type == GL_TYPE_ARRAY) {
    } else {
      printf(";\n");
    }
  }
}

gboolean
IsCommon(
  struct RecordTree *rec)
{
  if (!strcmp(rec->name,"state")) {
    return TRUE;
  }
  if (!strcmp(rec->name,"style")) {
    return TRUE;
  }
  if (!strcmp(rec->name,"visible")) {
    return TRUE;
  }
  return FALSE;
}

gboolean
CheckName(
  struct RecordTree *rec,
  const char *name)
{
  return !strcmp(rec->name,name);
}

void 
CheckPDF(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("objectdata");
      }
    }
  }
}

void 
CheckCList(
  struct RecordTree *rec)
{
  GList *list,*list2;
  struct RecordTree *child,*child2,*array,*item;
  int i;

  array = item = NULL;
  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (child->type == GL_TYPE_ARRAY) {

        for(list2 = child->children; list2 != NULL; list2 = list2->next) {
          child2 = (struct RecordTree*)list2->data;
          if (CheckName(child2,"item")) {
            item = child2;
          } else if (!IsCommon(child2) && 
                     !CheckName(child2,"count") &&
                     !CheckName(child2,"row") &&
                     !CheckName(child2,"rowattr") &&
                     !CheckName(child2,"column") &&
                     !CheckName(child2,"item") &&
                     !CheckName(child2,"fgcolor") &&
                     !CheckName(child2,"bgcolor")
          ) {
            child2->name = g_strdup("selectdata");
          }
        }

      }
    }
  }

  if (item == NULL || item->type != GL_TYPE_RECORD) {
    return;
  }
  for(i=0,list = item->children; list != NULL; list = list->next,i++) {
    child = (struct RecordTree*)list->data;
    child->name = g_strdup_printf("column%d",i+1);
  }
}

void
CheckTable(
  struct RecordTree *rec)
{
  GList *list,*list2;
  struct RecordTree *child,*child2,*array,*rowdata;
  int i;

  array = rowdata = NULL;
  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (child->type == GL_TYPE_ARRAY) {

        for(list2 = child->children; list2 != NULL; list2 = list2->next) {
          child2 = (struct RecordTree*)list2->data;
          if (CheckName(child2,"rowdata")) {
            rowdata = child2;
          }
        }

      }
    }
  }

  if (rowdata == NULL || rowdata->type != GL_TYPE_RECORD) {
    return;
  }
  for(i=0,list = rowdata->children; list != NULL; list = list->next,i++) {
    child = (struct RecordTree*)list->data;
    child->name = g_strdup_printf("column%d",i+1);
  }
}

void 
CheckHTML(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("uri");
      }
    }
  }
}

void
CheckEntry(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child) && !CheckName(child,"editable")) {
        child->name = g_strdup("textdata");
      }
    }
  }
}

void
CheckNumberEntry(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child) && !CheckName(child,"editable")) {
        child->name = g_strdup("numdata");
      }
    }
  }
}

void 
CheckText(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("textdata");
      }
    }
  }
}

void 
CheckPixmap(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("objectdata");
      }
    }
  }
}

void 
CheckLabel(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("textdata");
      }
    }
  }
}

void
CheckButton(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child) && !CheckName(child,"label")) {
        child->name = g_strdup("isactive");
      }
    }
  }
}

void 
CheckColorButton(
  struct RecordTree *rec)
{
  GList *list;
  struct RecordTree *child;

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      child = (struct RecordTree*)list->data;
      if (!IsCommon(child)) {
        child->name = g_strdup("color");
      }
    }
  }
}

void
_CheckRecordTree(
  GtkWidget *widget,
  struct RecordTree *rec)
{
  long type;

  type = (long)(G_OBJECT_TYPE(widget));
  if (type == GTK_PANDA_TYPE_PDF) {
    CheckPDF(rec);
  } else if (type == GTK_PANDA_TYPE_TABLE) {
    CheckTable(rec);
  } else if (type == GTK_PANDA_TYPE_CLIST) {
    CheckCList(rec);
  } else if (type == GTK_PANDA_TYPE_HTML) {
    CheckHTML(rec);
  } else if (type == GTK_PANDA_TYPE_ENTRY) {
    CheckEntry(rec);
  } else if (type == GTK_TYPE_NUMBER_ENTRY) {
    CheckNumberEntry(rec);
  } else if (type == GTK_PANDA_TYPE_TEXT) {
    CheckText(rec);
  } else if (type == GTK_PANDA_TYPE_PIXMAP) {
    CheckPixmap(rec);
  } else if (type == GTK_TYPE_LABEL) {
    CheckLabel(rec);
  } else if (type == GTK_TYPE_BUTTON) {
    CheckButton(rec);
  } else if (type == GTK_TYPE_TOGGLE_BUTTON) {
    CheckButton(rec);
  } else if (type == GTK_TYPE_CHECK_BUTTON) {
    CheckButton(rec);
  } else if (type == GTK_TYPE_RADIO_BUTTON) {
    CheckButton(rec);
  } else if (type == GTK_TYPE_COLOR_BUTTON) {
    CheckColorButton(rec);
  }
}

void
CheckRecordTree(
  GladeXML *xml,
  struct RecordTree *rec)
{
  GtkWidget *widget;
  GList *list;

  widget = glade_xml_get_widget_by_long_name(xml,rec->longname);
  if (widget != NULL) {
    _CheckRecordTree(widget,rec);
  }

  if (rec->type == GL_TYPE_RECORD) {
    for(list = rec->children; list != NULL; list = list->next) {
      CheckRecordTree(xml,(struct RecordTree*)list->data);
    }
  } else if (rec->type == GL_TYPE_ARRAY) {
    for(list = rec->children; list != NULL; list = list->next) {
      CheckRecordTree(xml,(struct RecordTree*)list->data);
    }
  }
}

int
main(int argc,char *argv[])
{
  GladeXML *xml;
  ValueStruct *val;
  struct RecordTree *top;
  char *vname;

  if (argc < 3) {
    fprintf(stderr,"./widgetpropconv window.glade window.rec\n");
    exit(-1);
  }


  RecParserInit();
  val = RecParseValue(argv[2],&vname);
  InitializeValue(val);

  top = MakeRecordTree(vname,vname,val);

  gtk_init(&argc,&argv);
  gtk_panda_init(&argc,&argv);
  glade_init();
  xml = glade_xml_new(argv[1],vname);

  CheckRecordTree(xml,top);

  OutputRecord(top,0);

  return 0;
}
