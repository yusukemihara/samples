/*
 *  gcc -g `pkg-config --cflags --libs glib-2.0 gconf-2.0` gconf.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <glib.h>
#include <gconf/gconf-client.h>

int
main(void)
{
	GConfClient *client;
	GSList *list,*l;
	
	client = gconf_client_get_default();
	gconf_client_set_string(client, "/apps/glclient/test", "hoge", NULL);
	fprintf(stderr,"[%s]\n",
		gconf_client_get_string(client, "/apps/glclient/test", NULL));
	fprintf(stderr,"[%s]\n",
		gconf_client_get_string(client, "/apps/glclient/server", NULL));
#if 0
	gconf_client_set_string(client, "/apps/glclient/servers/hoge/name", "hoge", NULL);
	gconf_client_set_string(client, "/apps/glclient/servers/fuga/name", "moge", NULL);
	gconf_client_set_string(client, "/apps/glclient/servers/gage/name", "がげ", NULL);
#endif
	list = gconf_client_all_dirs(client,"/apps/glclient/servers",NULL);
	for(l=list;l!=NULL;l=l->next) {
		fprintf(stderr,"[%s]\n",(char*)l->data);
		g_free(l->data);
	}
	g_slist_free(list);
    return 0;
}
