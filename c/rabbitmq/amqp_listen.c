#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs librabbitmq` -o $obj $src
exit
#endif

/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by Alan Antonuk are Copyright (c) 2012-2013
 * Alan Antonuk. All Rights Reserved.
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>


void die(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  exit(1);
}

void die_on_error(int x, char const *context)
{
  if (x < 0) {
    fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x));
    exit(1);
  }
}

void die_on_amqp_error(amqp_rpc_reply_t x, char const *context)
{
  switch (x.reply_type) {
  case AMQP_RESPONSE_NORMAL:
    return;

  case AMQP_RESPONSE_NONE:
    fprintf(stderr, "%s: missing RPC reply type!\n", context);
    break;

  case AMQP_RESPONSE_LIBRARY_EXCEPTION:
    fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x.library_error));
    break;

  case AMQP_RESPONSE_SERVER_EXCEPTION:
    switch (x.reply.id) {
    case AMQP_CONNECTION_CLOSE_METHOD: {
      amqp_connection_close_t *m = (amqp_connection_close_t *) x.reply.decoded;
      fprintf(stderr, "%s: server connection error %d, message: %.*s\n",
              context,
              m->reply_code,
              (int) m->reply_text.len, (char *) m->reply_text.bytes);
      break;
    }
    case AMQP_CHANNEL_CLOSE_METHOD: {
      amqp_channel_close_t *m = (amqp_channel_close_t *) x.reply.decoded;
      fprintf(stderr, "%s: server channel error %d, message: %.*s\n",
              context,
              m->reply_code,
              (int) m->reply_text.len, (char *) m->reply_text.bytes);
      break;
    }
    default:
      fprintf(stderr, "%s: unknown server error, method id 0x%08X\n", context, x.reply.id);
      break;
    }
    break;
  }

  exit(1);
}

static void dump_row(long count, int numinrow, int *chs)
{
  int i;

  printf("%08lX:", count - numinrow);

  if (numinrow > 0) {
    for (i = 0; i < numinrow; i++) {
      if (i == 8) {
        printf(" :");
      }
      printf(" %02X", chs[i]);
    }
    for (i = numinrow; i < 16; i++) {
      if (i == 8) {
        printf(" :");
      }
      printf("   ");
    }
    printf("  ");
    for (i = 0; i < numinrow; i++) {
      if (isprint(chs[i])) {
        printf("%c", chs[i]);
      } else {
        printf(".");
      }
    }
  }
  printf("\n");
}

static int rows_eq(int *a, int *b)
{
  int i;

  for (i=0; i<16; i++)
    if (a[i] != b[i]) {
      return 0;
    }

  return 1;
}

void amqp_dump(void const *buffer, size_t len)
{
  unsigned char *buf = (unsigned char *) buffer;
  long count = 0;
  int numinrow = 0;
  int chs[16];
  int oldchs[16] = {0};
  int showed_dots = 0;
  size_t i;

  for (i = 0; i < len; i++) {
    int ch = buf[i];

    if (numinrow == 16) {
      int i;

      if (rows_eq(oldchs, chs)) {
        if (!showed_dots) {
          showed_dots = 1;
          printf("          .. .. .. .. .. .. .. .. : .. .. .. .. .. .. .. ..\n");
        }
      } else {
        showed_dots = 0;
        dump_row(count, numinrow, chs);
      }

      for (i=0; i<16; i++) {
        oldchs[i] = chs[i];
      }

      numinrow = 0;
    }

    count++;
    chs[numinrow++] = ch;
  }

  dump_row(count, numinrow, chs);

  if (numinrow != 0) {
    printf("%08lX:\n", count);
  }
}
int main(int argc, char const *const *argv)
{
  char const *hostname;
  int port, status;
  char const *exchange;
  char const *bindingkey;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;

  amqp_bytes_t queuename;

  if (argc < 5) {
    fprintf(stderr, "Usage: amqp_listen host port exchange bindingkey\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);
  exchange = argv[3];
  bindingkey = argv[4];

  conn = amqp_new_connection();

  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }

  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 0, 1,
                                 amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      return 1;
    }
  }

  amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey),
                  amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

  {
    while (1) {
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;

      amqp_maybe_release_buffers(conn);

      res = amqp_consume_message(conn, &envelope, NULL, 0);

      if (AMQP_RESPONSE_NORMAL != res.reply_type) {
        break;
      }

      printf("Delivery %u, exchange %.*s routingkey %.*s\n",
             (unsigned) envelope.delivery_tag,
             (int) envelope.exchange.len, (char *) envelope.exchange.bytes,
             (int) envelope.routing_key.len, (char *) envelope.routing_key.bytes);

      if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
        char *msg;
        printf("Content-type: %.*s\n",
               (int) envelope.message.properties.content_type.len,
               (char *) envelope.message.properties.content_type.bytes);
        msg = malloc(envelope.message.body.len + 1);
        memset(msg,0,envelope.message.body.len+1);
        memcpy(msg,envelope.message.body.bytes,envelope.message.body.len);
        printf("Message: [%s]\n",msg);
        free(msg);
      }

      amqp_destroy_envelope(&envelope);
    }
  }

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  return 0;
}
