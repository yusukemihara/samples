#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs glib-2.0 gdk-pixbuf-2.0 gmodule-2.0` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

int
main(int argc, char *argv[])
{
  GdkPixbufFormat *format;
  GdkPixbuf *image;
  GError *error = NULL;
  gint w,h;
  int i;
  gchar *p;

  if (argc<2) {
    fprintf(stderr,"$ %s 1.png\n",argv[0]);
    exit(0);
  }

  format = gdk_pixbuf_get_file_info(argv[1],&w,&h);
  if (format != NULL) {
    fprintf(stderr,"name:        %s\n",gdk_pixbuf_format_get_name(format));
  } else {
    fprintf(stderr,"format = null\n");
  }

  image = gdk_pixbuf_new_from_file(argv[1],&error);
  if (image == NULL) {
    fprintf(stderr,"%s\n",error->message);
  }
  return 0;
}
