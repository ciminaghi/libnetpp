// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// server-ipv4-unix-socket.h: ipv4 servers via regular unix socket
#ifndef __server_ipv4_unix_socket_axtls_h__
#define __server_ipv4_unix_socket_axtls_h__

#include <vector>

#include <ssl.h>
#undef min
#undef max

#include "server-ipv4-unix-socket.h"
#include "host/connection-unix-socket-axtls.h"
#include "ssl-object.h"

namespace netpp {

  class stream_server_ipv4_unix_socket_axtls :
    public stream_server_ipv4_unix_socket {
  private:
    SSL_CTX *ssl_ctx;
  public:
    stream_server_ipv4_unix_socket_axtls(address_ipv4 &local,
					 vector<ssl_object *>&);
    stream_server_ipv4_unix_socket_axtls(unsigned short port,
					 vector<ssl_object *>&);
    virtual ~stream_server_ipv4_unix_socket_axtls();
  };
}

#endif // __server_ipv4_unix_socket_axtls_h__
