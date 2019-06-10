#if 0
gcc -g `pkg-config --cflags --libs libcurl json` $0
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <curl/curl.h>
#include <json.h>

#define	OUT_BUF_SIZE	1024

size_t write_data(
	void *buf,
	size_t size,
	size_t nmemb,
	void *userp)
{
	char outbuf[OUT_BUF_SIZE+1];
	char *p;
	size_t rest;

	p = buf;
	rest = size * nmemb;
	fprintf(stderr,"---- start\n");
	while (rest > 0) {
		if (rest > OUT_BUF_SIZE) {
			memcpy(outbuf,buf,OUT_BUF_SIZE);
			outbuf[OUT_BUF_SIZE] = 0;
			p += OUT_BUF_SIZE;
		} else {
			memcpy(outbuf,buf,rest);
			outbuf[rest] = 0;
			rest = 0;
		}
		fprintf(stderr,"%s",outbuf);
	}
	fprintf(stderr,"\n---- end\n");
	return size*nmemb;
}

size_t read_data(
	void *buf,
	size_t size,
	size_t nmemb,
	void *userp)
{
	char *p;
	static size_t send = 0;
    size_t psize;
    size_t rest;

	p = (char *)userp;
	psize = strlen(p) + 1;
fprintf(stderr,"p[%s]\n",p);

	p += send;
	rest = psize - send;

	if (rest > size * nmemb) {
		memcpy(buf,p,size * nmemb);
		send += size * nmemb;
		rest = size * nmemb;
	} else {
		memcpy(buf,p,rest);
		send += rest;
	}
	return rest;
}

size_t
test_header(
	void *ptr,
	size_t size,
	size_t nmemb,
	void *userdata)
{
	size_t all = size * nmemb;
	int *oid;
	char *buf;
	char *target = "Content-Length:";
	char *p;
	int i;

	oid = (int*)userdata;
	if (all <= strlen(target)) {
		return all;
	}

	if (strncmp(ptr,target,strlen(target))) {
		return all;
	}
	buf = malloc(all+1);
	for(p=ptr+strlen(target);isspace(*p);p++);
	for(i=0;isdigit(*p);p++,i++) {
		buf[i] = *p;
	}
	buf[i] = 0;
	*oid = atoi(buf);
	free(buf);
fprintf(stderr,"oid[%d]\n",*oid);
	return all;
}

CURLcode post(
	char *url)
{
	FILE *fp;
	CURL *curl;
	CURLcode ret;
	struct curl_slist *headers = NULL;
	json_object *obj,*child;
	int oid;

	obj = json_object_new_object();
	child = json_object_new_object();
	json_object_object_add(child,"a",json_object_new_string("b"));
	json_object_object_add(obj,"child",child);

	fprintf(stderr,"[%s]\n",json_object_to_json_string(obj));

	ret = CURLE_COULDNT_CONNECT;
	curl = curl_easy_init();
	if (!curl) {
		printf("couldn't init curl\n");
		return ret;
	}
	fp = fopen("/tmp/moge", "w");
    if (fp == NULL) {
		fprintf(stderr,"cannot open write file\n");
	}

	headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void *)json_object_to_json_string(obj));
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data);
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "sample:sample");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,test_header);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER,(void*)&oid);
	ret = curl_easy_perform(curl);
	curl_slist_free_all(headers);
	fclose(fp);
	if (ret == CURLE_OK) {
		fprintf(stderr,"success\n");
	} else {
		fprintf(stderr,"fail\n");
	}
	curl_easy_cleanup(curl);

	json_object_put(obj);
	return ret;
}

int main(int argc,char *argv[])
{
	int i;

	if (argc < 2) {
		return 0;
	}
	post(argv[1]);
	return 0;
}
