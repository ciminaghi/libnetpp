// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// client-ipv4.h: ipv4 client abstraction
#ifndef __client_ipv4_h__
#define __client_ipv4_h__

#include "address-ipv4.h"
#include "client.h"

namespace netpp {

  class stream_client_ipv4 : public stream_client {
  public:
    stream_client_ipv4(const address_ipv4 &remote, connection *c = NULL):
      stream_client(remote, c) { };
    virtual ~stream_client_ipv4() { };
  };

  class dgram_client_ipv4 : public dgram_client {
  public:
    dgram_client_ipv4(const address_ipv4 &remote, connection *c = NULL);
    virtual ~dgram_client_ipv4() { };
  };

} // namespace netpp

#endif // __client_ipv4_h__
