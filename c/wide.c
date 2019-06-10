#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

main()
{
	char buf[1000];
	wchar_t wbuf[1000];
	int j, len;

	setlocale(LC_ALL, "");
	
	fgets(buf, 1000, stdin);
	len = strlen(buf);
	if (len > 0) buf[len-1] = 0;   /* '\n' をとりのぞく */
	
	printf("Original Text = \'%s\'\n", buf);

	/* multi-byte string -> wide charcter string */
	mbstowcs(wbuf, buf, 1000);

	for(j=0; wbuf[j]; j++) {
		len = wctomb(buf, wbuf[j]);
		if (len>0) {
			buf[len] = 0;
			printf("%d .. len=%d \'%s\'\n", j, len, buf);
		} else {
			printf("%d .. len=%d\n", j, len);
		}
	}
	return 0;
}
