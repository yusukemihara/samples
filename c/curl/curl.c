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
	FILE *fp;

	fp = (FILE*)userp;
	wrote_size += size * nmemb;
	return fwrite(buf,size,nmemb,fp);
}

#if 1
CURLcode dl_print_data(
	char *url,
	char *title)
{
	FILE *fp;
	int fd;
	mode_t mode;
	char fname[256];
	CURL *curl;
	CURLcode ret;

	ret = CURLE_COULDNT_CONNECT;
	curl = curl_easy_init();
	if (!curl) {
		printf("couldn't init curl\n");
		return ret;
	}

	sprintf(fname,"%s/glclient_print_XXXXXX","/tmp");

	mode = umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if ((fd = mkstemp(fname)) != -1) {
		if ((fp = fdopen(fd, "w")) != NULL) {
			wrote_size = 0;
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)fp);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			curl_easy_setopt(curl, CURLOPT_USERPWD, "sample:sample");
			ret = curl_easy_perform(curl);
			fclose(fp);
			if (ret == CURLE_OK && wrote_size != 0) {
				//show_dialog
fprintf(stderr,"url[%s] fname[%s] size:[%ld]\n",url,fname,(long)wrote_size);
			}
			curl_easy_cleanup(curl);
		} else {
			//Warning("fdopne failure");
		}
	} else {
		//Warning("mkstemp failure");
	}
	umask(mode);
	return ret;
}
#endif

int main(int argc,char *argv[])
{
	int i;

	if (argc < 3) {
		return 0;
	}
	for (i = 1; i< argc; i++) {
		dl_print_data(argv[i],argv[i]);
	}
	return 0;
}
