// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <streambuf>
#include "netpp.h"
#include "address-socket.h"
#include "host/connection-unix-socket-axtls.h"
#include "host/event-manager.h"

using namespace netpp;
using namespace std;

// connection-unix-socket-axtls.cpp: secure (axtls) unix socket connection
// implementation

namespace netpp {

  // event for secure passive connections


  event *secure_passive_connection_event_unix_socket::get(stream_server &r,
							  address_socket& l,
							  int fd,
							  SSL_CTX *_ctx)
  {
    return new secure_passive_connection_event_unix_socket(r, l, fd, _ctx);
  }

  // unix socket axtls connection
  connection_unix_socket_axtls::
  connection_unix_socket_axtls(address &l, address &r, int id,
			       client_server& _o, SSL *_ssl):
    connection(l, r, id, _o, new connection_axtls_sb(_ssl)), ssl(_ssl)
  {
    linux_event_manager::get()->add_fd(id, rx_event_axtls::get(_o, this));
  }

  connection_unix_socket_axtls::
  connection_unix_socket_axtls(address &l, int _id, client_server& _o,
				 SSL *_ssl):
    connection(l, address::unknown, _id, _o, new connection_axtls_sb(_ssl)),
    ssl(_ssl)
    {
      linux_event_manager::get()->add_fd(_id, rx_event_axtls::get(_o, this));
    }

  connection_unix_socket_axtls::
  connection_unix_socket_axtls(int _id, client_server& _o, SSL *_ssl):
    connection(address_ipv4::any, address::unknown, _id, _o,
	       new connection_axtls_sb(_ssl)), ssl(_ssl)
  {
    linux_event_manager::get()->add_fd(_id, rx_event_axtls::get(_o, this));
  }

  // rx event for secure connections
  void rx_event_axtls::doit(void)
  {
    if (!c->handshaking_ok()) {
      c->dummy_read();
      return;
    }
    rx_event::doit();
  }
  
} // namespace netpp
