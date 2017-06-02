// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// client-ipv4-unix-socket.h: ipv4 clients via regular unix socket
#ifndef __client_ipv4_unix_socket_h__
#define __client_ipv4_unix_socket_h__

#include "client-ipv4.h"

namespace netpp {

  class stream_client_ipv4_unix_socket : public stream_client_ipv4 {
  private:
    int fd;
    event *e;
    address_ipv4 &remote_ipv4;
    address_ipv4 &local_ipv4;
  public:
    stream_client_ipv4_unix_socket(address_ipv4 &remote);
    virtual ~stream_client_ipv4_unix_socket();
  };

  class dgram_client_ipv4_unix_socket : public dgram_client_ipv4 {
  private:
    int fd;
    address_ipv4 &remote_ipv4;
    address_ipv4 &local_ipv4;
  public:
    dgram_client_ipv4_unix_socket(address_ipv4 &remote);
    virtual ~dgram_client_ipv4_unix_socket();
  };

} // namespace netpp

#endif // __client_ipv4_unix_socket_h__
