/*
 * gcc -g `pkg-config --cflags glib-2.0` g_regex_match.c  `pkg-config --libs glib-2.0`
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

void
match(char *pat,char *str)
{
	int compile_op,match_op;

	compile_op = G_REGEX_DOTALL | G_REGEX_MULTILINE;
	match_op = 0;
	if (g_regex_match_simple(pat,str,compile_op,match_op)) {
		printf("match\n  |%s|\n  |%s|\n\n",pat,str);
	} else {
		printf("unmatch\n  |%s|\n  |%s|\n\n",pat,str);
	}
}

int 
main(
	int argc,
	char *argv[])
{
	char str[1024],*pat;

	pat = "^\\s*({.*}|\\[.*\\])\\s*$";

	sprintf(str,"    {\naaaa\n } ");
	match(pat,str);

	sprintf(str,"    [\naaaa\n ] ");
	match(pat,str);

	sprintf(str," dfadf   \naaaa\n } ");
	match(pat,str);

	sprintf(str,"    {aaaa } ");
	match(pat,str);

	sprintf(str,"    [aaaa ] ");
	match(pat,str);

	sprintf(str,"    [aaaa } ");
	match(pat,str);
	return 0;
}
