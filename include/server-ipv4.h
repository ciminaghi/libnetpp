// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// server-ipv4.h: ipv4 server abstraction
#ifndef __server_ipv4_h__
#define __server_ipv4_h__

#include "server.h"
#include "address-ipv4.h"

namespace netpp {

  class stream_server_ipv4 : public stream_server {
  public:
    stream_server_ipv4(const address_ipv4 &local) : stream_server(local) { };
    stream_server_ipv4(unsigned short port) :
      stream_server(address_ipv4(INADDR_ANY, port)) { };
    virtual void handle(void) { };
    virtual ~stream_server_ipv4() { };
  };

  class dgram_server_ipv4 : public dgram_server {
  public:
    dgram_server_ipv4(const address_ipv4 &local) : dgram_server(local) { };
    dgram_server_ipv4(unsigned short port);
    virtual void handle(void) { };
    virtual ~dgram_server_ipv4() { };
  };

} // namespace netpp

#endif // __server_ipv4_h__

