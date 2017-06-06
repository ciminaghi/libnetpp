// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "netpp.h"
#include "address-ipv4.h"

#include "host/server-ipv4-unix-socket.h"
#include "host/event-manager.h"
#include "host/connection-unix-socket.h"

using namespace netpp;
using namespace std;

void stream_server_ipv4_unix_socket::create_socket(void)
{
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    fatal("socket error", 127);
  int enable = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    fatal("setsockopt(SO_REUSEADDR) failed", 127);
}

void stream_server_ipv4_unix_socket::
bind_and_listen(const struct sockaddr_in *a)
{
  if (bind(fd, (struct sockaddr *)a, sizeof(*a)) < 0)
    fatal("bind error", 127);
  if (listen(fd, 5) < 0)
    fatal("listen error", 127);
}

void stream_server_ipv4_unix_socket::setup_event(void)
{
  e = passive_connection_event_unix_socket::get(*this, local_ipv4, fd);
  linux_event_manager *em = linux_event_manager::get();
  em->add_fd(fd, e);
}

stream_server_ipv4_unix_socket::
stream_server_ipv4_unix_socket(address_ipv4 &local, bool defer_evt_setup) :
  stream_server_ipv4(local), local_ipv4(local), e(nullptr)
{
  create_socket();
  struct sockaddr_in a;
  local.read(a);
  bind_and_listen(&a);
  if (!defer_evt_setup)
    setup_event();
}

stream_server_ipv4_unix_socket::
stream_server_ipv4_unix_socket(unsigned short port, bool defer_evt_setup) :
  stream_server_ipv4(port), local_ipv4(address_ipv4::any), e(nullptr)
{
  create_socket();
  struct sockaddr_in a;
  a.sin_family = AF_INET;
  a.sin_port = htons(port);
  a.sin_addr.s_addr = INADDR_ANY;
  bind_and_listen(&a);
  if (!defer_evt_setup)
    setup_event();
}

stream_server_ipv4_unix_socket::
~stream_server_ipv4_unix_socket()
{
  linux_event_manager *em = linux_event_manager::get();
  em->del_fd(fd);
  close(fd);
}
