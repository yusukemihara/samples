/*
 * redconv3.c
 * % gcc `pkg-config --libs --cflags glib-2.0 libxml-2.0` -lz -g redconv3.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>

#include <glib.h>
#include <zlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#define GZ_MODE "rb6f"
#define BUF_SIZE (512)

gchar *
ReadRed(gchar *fname)
{
  gint fd;
  gzFile gzfile;
  gsize readbytes;
  gsize size;
  gchar buf[BUF_SIZE];
  gchar *ret;

  fd = g_open(fname,O_RDONLY);
  if (fd < 0) {
     return g_strdup("");
  }
  gzfile = gzdopen(fd, GZ_MODE);
  if (gzfile != NULL) {
    // dummy read to get deflate size
    size = 0;
	while((readbytes = gzread(gzfile, buf, sizeof(buf))) > 0 ) {
      size += readbytes;
    }
    gzrewind(gzfile);
    ret = g_malloc(size + 1);
    ret[size] = 0;
    gzread(gzfile,ret,size);
    gzclose(gzfile);
  } else {
    // normal read
    gchar *buf;
    g_file_get_contents(fname,&buf,&size,NULL);
    ret = g_realloc(buf,size + 1);
    ret[size] = 0;
    g_free(buf);
  }
  return ret;
}

static gboolean
eval_cb(
  const GMatchInfo *info,
  GString *res,
  gpointer data)
{
  gchar *match;
  guchar c[2];

  match = g_match_info_fetch(info, 1);
  c[0] = (guchar)atoi(match);
  c[1] = 0;
  g_string_append (res, c);
  g_free(match);
  return FALSE;
}

char *
ConvertRed(gchar *in)
{
  gchar *buf1;
  gchar *buf2;
  GRegex *reg;
  gboolean match = FALSE;
  GError *err = NULL;
  gsize br,bw;

  reg = g_regex_new("&#([0-9]+);",G_REGEX_MULTILINE,0,NULL); 
  if (g_regex_match(reg,in,0,NULL)) {
    buf1 = g_regex_replace_eval(reg,in,-1,0,0,eval_cb,&match,&err);
    if (err != NULL) {
      buf1 = in;
      g_error_free(err);
      err = NULL;
    }
    buf2 = g_convert(buf1,-1,"utf-8", "euc-jisx0213",&br, &bw, &err);
    if (err != NULL) {
      buf2 = g_strdup("");
      g_error_free(err);
      err = NULL;
    }
    g_free(buf1);
    return buf2;
  }
  return in;
}

#define MONPE_XML_EMBED_TEXTOBJ_ATTR_COLUMN "embed_text_column"
static void
ConvertEmbedTextColumn(xmlNodePtr root)
{
  xmlNodePtr node;
  xmlChar *val;
  xmlChar *name;
  xmlChar replace[10];
  gint column;

  if (!xmlStrcmp(root->name, "attribute")) {
     name = xmlGetProp(root, "name");
     if (name != NULL && !xmlStrcmp(name,MONPE_XML_EMBED_TEXTOBJ_ATTR_COLUMN)) {
       xmlFree(name);
       node = root->children;
       if (!xmlStrcmp(node->name, "int")) {
         val = xmlGetProp(node, "val");
         if (val != NULL) {
           column = atoi(val) / 2;
           column = column >= 0 ? column : 1;
           snprintf(replace,sizeof(replace),"%d",column);
           xmlSetProp(node, "val", replace);
           xmlFree(val);
         }
       }
     }
  }

  for(node = root->children; node != NULL; node = node->next) {
    ConvertEmbedTextColumn(node);
  }
}

int 
main(int argc,char *argv[])
{
  xmlDocPtr doc;
  xmlNodePtr root;
  xmlNsPtr namespace;
  gchar *buf1;
  gchar *buf2;

  buf1 = ReadRed(argv[1]);
  buf2 = ConvertRed(buf1);
  g_free(buf1);

  doc = xmlReadMemory((const char*)buf2,strlen(buf2),
    "http://www.montsuqi.org/",NULL, XML_PARSE_NOBLANKS|XML_PARSE_NOENT);
  g_free(buf2);
  if (doc == NULL) {
    return 0;
  }

  root = xmlDocGetRootElement(doc);

  namespace = xmlSearchNs(doc, root, "monpe2");
  if (namespace == NULL) { /* under compatible */
    namespace = xmlSearchNs(doc, root, "dia");
  }
  if (namespace == NULL) { /* under compatible */
    namespace = xmlSearchNs(doc, root, "monpe");
  }
  
  if (xmlStrcmp (root->name, "diagram") || (namespace == NULL)){
    xmlFreeDoc (doc);
    return 0;
  }

  if (xmlStrcmp(namespace->prefix,"monpe2")) {
    ConvertEmbedTextColumn(root);
  }
  return 0;
}
