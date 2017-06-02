// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// client-server.h: common stuff among clients and servers
#ifndef __client_server_h__
#define __client_server_h__

#include "event.h"
#include "address.h"

namespace netpp {

  class client_server : public event_receiver {
  public:
    client_server() { };
    // Invoked by rx event
    virtual void data_available(connection *) = 0;
    virtual void disconnected(connection *) = 0;
    virtual ~client_server() { }
  };

  // This represents a generic (stream OR dgram) server
  class server : public client_server {
  protected:
    const address &local;
  public:
    server(const address &l): client_server(), local(l) { };
    server(const server &s): client_server(), local(s.get_local()) { };
    virtual bool is_stream(void) = 0;
    const address &get_local(void) const { return local ; };
    virtual ~server() { };
  };

  // This represents a generic (stream OR dgram) client
  class client : public client_server {
  protected:
    const address &remote;
    connection *conn;
  public:
    client(const address &r, connection *c):
      client_server(), remote(r), conn(c) { };
    client(const client &c):
      client_server(), remote(c.get_remote()), conn(c.get_connection()) { };
    virtual bool is_stream(void) = 0;
    const address &get_remote(void) const { return remote ; };
    connection *get_connection(void) const { return conn; };
    void set_connection(connection *c) { conn = c; };
    virtual ~client() { };
  };

};

#endif // __client_server_h__



