#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -o $obj $src `pkg-config --cflags --libs ibus-1.0 dbus-1 glib-2.0`
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ibus.h>
#include <glib.h>
#include <dbus/dbus.h>

static GDBusConnection *
get_ibus_connect(void)
{
  GDBusConnection *connect = NULL;

  if ( ibus_get_address() != NULL) {
    connect = g_dbus_connection_new_for_address_sync (ibus_get_address (),
						      G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT |
						      G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION,
						      NULL, NULL, NULL);
  }
  return connect;
}

static gchar *
get_ibus_CurrentInputContext(
			     GDBusConnection *connect)
{
  gchar *current = NULL;
  GVariant *result;

  result = g_dbus_connection_call_sync (connect,
					IBUS_SERVICE_IBUS,
					IBUS_PATH_IBUS,
					IBUS_INTERFACE_IBUS,
					"CurrentInputContext",
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);
  g_variant_get(result,"(o)", &current);
  g_variant_unref (result);
  return current;
}

gboolean
is_ibus_enable(
	       GDBusConnection *connect,
	       gchar *current)
{
  gboolean bool;
  GVariant *result;

  result = g_dbus_connection_call_sync (connect,
					IBUS_SERVICE_IBUS,
					current,
					IBUS_INTERFACE_INPUT_CONTEXT,
					"IsEnabled",
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);

  g_variant_get(result,"(b)", &bool);
  g_variant_unref (result);
  return bool;
}

void
ibus_change_state(
		  GDBusConnection *connect,
		  gchar *current,
		  const gchar *state)
{
  GVariant *result;
  result = g_dbus_connection_call_sync (connect,
					IBUS_SERVICE_IBUS,
					current,
					IBUS_INTERFACE_INPUT_CONTEXT,
					state,
					NULL,
					NULL,
					G_DBUS_CALL_FLAGS_NO_AUTO_START,
					-1,
					NULL,
					NULL);
  g_variant_unref (result);
}

int
main ()
{
  GDBusConnection *connect;
  gchar *current = NULL;

  g_type_init ();

  connect = get_ibus_connect();

  int i;
  for (i=0;i<10;i++) {
    current = get_ibus_CurrentInputContext(connect);
    if (is_ibus_enable(connect, current)) {
      ibus_change_state(connect, current, "Disable");
    } else {
      ibus_change_state(connect, current, "Enable");
    }
    sleep(1);
  }
  g_dbus_connection_close_sync(connect,NULL,NULL);
  g_object_unref(connect);
  return 0;
}

