#if 0
gcc -g `pkg-config --cflags --libs libcurl` $0
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <curl/curl.h>

static size_t wrote_size = 0;

size_t write_data(
	void *buf,
	size_t size,
	size_t nmemb,
	void *userp)
{
	char *out;
	
	out = malloc(size*nmemb+1);
	memcpy(out,buf,size*nmemb);
	out[size*nmemb] = 0;
	fprintf(stderr,"%s",out);
	return size*nmemb;
}

void
get(
	char *url)
{
	CURL *curl;
	CURLcode code;

	code = CURLE_COULDNT_CONNECT;
	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr,"couldn't init curl\n");
		return;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	if (!strncmp("https",url,5)) {
		curl_easy_setopt(curl,CURLOPT_USE_SSL,CURLUSESSL_ALL);
		curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,1);
		curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	code = curl_easy_perform(curl);
	fprintf(stderr,"url[%s] code[%d]\n",url,code);
	curl_easy_cleanup(curl);
}

int 
main(int argc,char *argv[])
{
	int i;

	for (i = 1; i< argc; i++) {
		get(argv[i]);
	}
	return 0;
}
