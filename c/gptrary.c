/* gcc -I/usr/include/glib-1.2 -I /usr/lib/glib/include -lglib glistindex.c*/

#include <stdio.h>
#include <glib.h>

int main()
{
    int i;
	GPtrArray *ary;
    ary = g_ptr_array_new();
    g_ptr_array_add(ary, "a");
    g_ptr_array_add(ary, "b");
    g_ptr_array_add(ary, "c");
    g_ptr_array_add(ary, "d");
    g_ptr_array_add(ary, "e");
    g_ptr_array_add(ary, "f");
    g_ptr_array_remove(ary,"d");
    for(i=0;i<10;i++){
		printf("[%d] [%s]\n",i,g_ptr_array_index(ary,i));    
    }
	return 0;
}
