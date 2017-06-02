// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "netpp.h"
#include "connection.h"
#include "address-ipv4.h"

#include "host/client-ipv4-unix-socket.h"
#include "host/event-manager.h"
#include "host/connection-unix-socket.h"

using namespace netpp;
using namespace std;

stream_client_ipv4_unix_socket::
stream_client_ipv4_unix_socket(address_ipv4 &remote) :
  stream_client_ipv4(remote), remote_ipv4(remote), local_ipv4(address_ipv4::any)
{
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    fatal("error creating socket", 127);
  struct sockaddr_in a;
  remote.read(a);
  if (connect(fd, (struct sockaddr *)&a, sizeof(a)) < 0)
    fatal("error connecting to server", 127);
  struct sockaddr_in l; socklen_t len = sizeof(l);
  if (getsockname(fd, (struct sockaddr *)&l, &len) < 0)
    log_warn << "getsockname() returns error" << endl;
  local_ipv4 = address_ipv4(l);
  connection_unix_socket *c = new connection_unix_socket(local_ipv4,
							 remote, fd, *this);
  set_connection(c);
  linux_event_manager *e = linux_event_manager::get();
  e->add_fd(fd, rx_event::get(*this, c));
}

stream_client_ipv4_unix_socket::
~stream_client_ipv4_unix_socket()
{
  linux_event_manager *e = linux_event_manager::get();
  e->del_fd(fd);
  close(fd);
}
