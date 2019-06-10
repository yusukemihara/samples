/*
 * gcc -g `pkg-config --cflags glib-2.0` g_regex_match.c  `pkg-config --libs glib-2.0`
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

int 
main(
	int argc,
	char *argv[])
{
	GRegex *re;
	GMatchInfo *match;
	gchar *str;

	re = g_regex_new("^([Ee][Tt]|[Oo][Ss][Tt])\\s+/([a-zA-Z0-9]+)/([a-zA-Z0-9]+)(/*|\\?([a-zA-Z0-9&=]+))\\s",0,0,NULL);
	str = "ost  /ld/window?moge=noge&goge=guga  HTTP";
	if (g_regex_match(re,str,0,&match)) {
		gchar *ld = g_match_info_fetch(match,2);
		gchar *window = g_match_info_fetch(match,3);
		gchar *args = g_match_info_fetch(match,5);
		fprintf(stderr,"%s %s %s\n",ld,window,args);
		g_free(ld);
		g_free(window);
		g_free(args);
		g_match_info_free(match);
	} else {
	}
	g_regex_unref(re);
	return 0;
}
