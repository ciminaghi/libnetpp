// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// server-ipv4-unix-socket.h: ipv4 servers via regular unix socket
#ifndef __server_ipv4_unix_socket_h__
#define __server_ipv4_unix_socket_h__

#include "server-ipv4.h"

namespace netpp {

  class stream_server_ipv4_unix_socket : public stream_server_ipv4 {
  protected:
    int fd;
    void create_socket(void);
    void bind_and_listen(const struct sockaddr_in *);
    void setup_event(void);
    address_ipv4 &local_ipv4;
    event *e;
  public:
    stream_server_ipv4_unix_socket(address_ipv4 &local,
				   bool defer_evt_setup = false);
    stream_server_ipv4_unix_socket(unsigned short port,
				   bool defer_ebt_setup = false);
    virtual ~stream_server_ipv4_unix_socket();
  };

  class dgram_server_ipv4_unix_socket : public dgram_server_ipv4 {
  public:
    dgram_server_ipv4_unix_socket(address_ipv4 &local);
    dgram_server_ipv4_unix_socket(unsigned short port);
    virtual ~dgram_server_ipv4_unix_socket();
  };

}

#endif // __server_ipv4_unix_socket_h__
