// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// client.h: client abstraction and relevant events
#ifndef __client_h__
#define __client_h__

#include "event.h"
#include "client-server.h"

namespace netpp {

  // Basic client class is declared in client-server.h
  class stream_client : public client {
  public:
    stream_client(const address& remote, connection *c) : client(remote, c) { };
    virtual bool is_stream(void) { return true; };
    virtual ~stream_client() { };
  };

  class dgram_client : public client {
  public:
    dgram_client(const address &remote, connection *c) : client(remote, c) { };
    virtual bool is_stream(void) { return false ; };
    virtual ~dgram_client() { };
  };

} // namespace netpp


#endif // __client_h__

