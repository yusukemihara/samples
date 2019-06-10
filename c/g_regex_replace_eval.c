/*
 * gcc -g `pkg-config --cflags --libs glib-2.0` g_regex_replace_eval.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

static gboolean
eval_cb1(
	const GMatchInfo *info,
	GString *res,
	gpointer data)
{
	gchar *match;

	match = g_match_info_fetch(info, 1);
	if (!strcmp(match,"\xEF\xBC\x8D")) {
		g_string_append (res, "\xE2\x88\x92");
	} else if (!strcmp(match,"\xEF\xBD\x9E")) {
		g_string_append (res, "\xE3\x80\x9C");
	} else if (!strcmp(match,"\xE2\x88\xA5")) {
		g_string_append (res, "\xE2\x80\x96");
	} else if (!strcmp(match,"\xEF\xBF\xA0")) {
		g_string_append (res, "\xC2\xA2");
	} else if (!strcmp(match,"\xEF\xBF\xA1")) {
		g_string_append (res, "\xC2\xA3");
	} else if (!strcmp(match,"\xEF\xBF\xA2")) {
		g_string_append (res, "\xC2\xAC");
	}
	g_free(match);
	return FALSE;
}

static gchar*
filter(
	gchar *str)
{
    GRegex *reg;
	GError *err = NULL;
	gchar *ret;

	reg = g_regex_new("([" 
		"\xEF\xBC\x8D"
		"\xEF\xBD\x9E"
		"\xE2\x88\xA5"
		"\xEF\xBF\xA0"
		"\xEF\xBF\xA1"
		"\xEF\xBF\xA2"
		"])",0,0,NULL);

	ret = g_regex_replace_eval(reg,str,-1,0,0,eval_cb1,NULL,&err);
    if (err != NULL) {
		g_error("%s %p",err->message,ret);
    }
	if (ret == NULL) {
		ret = g_strdup(str);
	}
	return ret;
}

int main(int argc,char *argv[])
{
	fprintf(stderr,"%s\n",filter((gchar*)main));
	return 0;
}
