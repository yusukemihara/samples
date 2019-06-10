#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -o $obj $src -g `pkg-config --cflags --libs libwebsockets uuid`
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <uuid/uuid.h>

#define BUF_SIZE (10*1024)

static struct lws *wsi_pr;
static volatile int force_exit;
static char reqid[64];

static int
websocket_write_back(
	struct lws *wsi_in, 
	char *str, 
	int str_size_in) 
{
	int n;
	int len;
	char *out = NULL;

	if (str == NULL || wsi_in == NULL) {
		return -1;
	}
	
	if (str_size_in < 1) {
		len = strlen(str);
	} else {
		len = str_size_in;
	}
	
	out = (char *)malloc(sizeof(char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
	memcpy (out + LWS_SEND_BUFFER_PRE_PADDING, str, len );
	n = lws_write(wsi_in, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);
	free(out);
	
	return n;
}

static int
callback_push_receive(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	char buf[BUF_SIZE];

	switch (reason) {

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		fprintf(stderr,"LWS_CALLBACK_CLIENT_ESTABLISHED\n");
		/* subscribe */
		snprintf(buf,sizeof(buf)-1,"{\"command\":\"subscribe\",\"req.id\":\"%s\",\"event\":\"*\"}",reqid);
		websocket_write_back(wsi, buf, -1);
		break;

	case LWS_CALLBACK_CLOSED:
		fprintf(stderr,"LWS_CALLBACK_CLOSED\n");
		wsi_pr = NULL;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		((char *)in)[len] = '\0';
		fprintf(stderr,"LWS_CALLBACK_CLIENT_RECEIVE\n");
		fprintf(stderr,"%s\n", (char *)in);
		break;

	/* because we are protocols[0] ... */

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		if (wsi == wsi_pr) {
			fprintf(stderr,"LWS_CALLBACK_CLIENT_CONNECTION_ERROR:%s\n",(char*)in);
			wsi_pr = NULL;
		}
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		fprintf(stderr,"LWS_CALLBACK_CLIENT_WRITEABLE\n");
		break;

	case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
		if ((strcmp(in, "deflate-stream") == 0) ) {
			fprintf(stderr,"denied deflate-stream extension\n");
			return 1;
		}
		if ((strcmp(in, "x-webkit-deflate-frame") == 0))
			return 1;
		if ((strcmp(in, "deflate-frame") == 0))
			return 1;
		break;

	default:
		break;
	}

	return 0;
}


/* list of supported protocols and callbacks */
static struct lws_protocols protocols[] = {
	{
		"",
		callback_push_receive,
		0,
		BUF_SIZE,
	},
	{ NULL, NULL, 0, 0 } /* end */
};

#if 0
static const struct lws_extension exts[] = {
	{
		"permessage-deflate",
		lws_extension_callback_pm_deflate,
		"permessage-deflate; client_max_window_bits"
	},
	{
		"deflate-frame",
		lws_extension_callback_pm_deflate,
		"deflate_frame"
	},
	{ NULL, NULL, NULL /* terminator */ }
};
#else
static const struct lws_extension exts[] = {
	{ NULL, NULL, NULL /* terminator */ }
};
#endif

static int ratelimit_connects(unsigned int *last, unsigned int secs)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if (tv.tv_sec - (*last) < secs)
		return 0;

	*last = tv.tv_sec;

	return 1;
}

int main(int argc, char **argv)
{
	int n = 0, ret = 0 , use_ssl = 0, ietf_version = -1;
	unsigned int rl_pr = 0;
	struct lws_context_creation_info info;
	struct lws_client_connect_info i;
	struct lws_context *context;
	const char *prot, *p;
	char path[300], uri[256];
	uuid_t u;

	uuid_generate(u);
	uuid_unparse(u,reqid);

	snprintf(uri,sizeof(uri)-1,"ws://localhost:9400/ws");

	memset(&info, 0, sizeof info);
	memset(&i, 0, sizeof(i));

	i.port = 9400;
	if (lws_parse_uri((char*)uri, &prot, &i.address, &i.port, &p)) {
		fprintf(stderr,"lws_parse_uri error\n");
		exit(1);
	}

	/* add back the leading / on path */
	path[0] = '/';
	strncpy(path + 1, p, sizeof(path) - 2);
	path[sizeof(path) - 1] = '\0';
	i.path = path;

	if (!strcmp(prot, "http") || !strcmp(prot, "ws")) {
		use_ssl = 0;
	}
	if (!strcmp(prot, "https") || !strcmp(prot, "wss")) {
		use_ssl = 1;
	}

	/*
	 * create the websockets context.  This tracks open connections and
	 * knows how to route any traffic and which protocol version to use,
	 * and if each connection is client or server side.
	 *
	 * For this client-only demo, we tell it to not listen on any port.
	 */

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	context = lws_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	i.context = context;
	i.ssl_connection = use_ssl;
	i.host = i.address;
	i.origin = i.address;
	i.ietf_version_or_minus_one = ietf_version;
	i.client_exts = exts;

fprintf(stderr,"use_ssl:%d\n",use_ssl);
fprintf(stderr,"host:%s\n",i.address);
fprintf(stderr,"port:%d\n",i.port);
fprintf(stderr,"path:%s\n",path);
	/*
	 * sit there servicing the websocket context to handle incoming
	 * packets, and drawing random circles on the mirror protocol websocket
	 *
	 * nothing happens until the client websocket connection is
	 * asynchronously established... calling lws_client_connect() only
	 * instantiates the connection logically, lws_service() progresses it
	 * asynchronously.
	 */

	while (!force_exit) {

		if (!wsi_pr && ratelimit_connects(&rl_pr, 2u)) {
			lwsl_notice("connecting\n");
			i.protocol = protocols[0].name;
			wsi_pr = lws_client_connect_via_info(&i);
		}

		lws_service(context, 500);
	}

	lwsl_err("Exiting\n");
	lws_context_destroy(context);

	return ret;
}
