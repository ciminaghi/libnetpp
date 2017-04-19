// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection.h: connection abstraction and relevant events
#ifndef __connection_h__
#define __connection_h__

#include <iostream>
#include <map>
#include <streambuf>
using namespace std;


#include "event.h"
//#include "address.h"
#include "server.h"
#include "client.h"

namespace netpp {

  // This represents a connection with a remote peer. It can be assigned to a
  // stream server. It is derived from a streambuf, so that overflow and
  // underflow methods can be reimplemented
  class connection : public iostream {
  private:
    address &local, &remote;
    client_server& owner;
    int id;
  protected:
    void set_local(const address& a) { local = a; };
    void set_remote(const address& a) { remote = a; };
  public:
    connection(address &l, address &r, int _id,
	       client_server& _o, streambuf *sb) :
      iostream(sb), local(l), remote(r), owner(_o), id(_id) { };
    connection(address &l, int _id, client_server& _o, streambuf *sb):
      iostream(sb), local(l), remote(address::unknown), owner(_o), id(_id) { };
    connection(int _id, client_server& _o, streambuf *sb):
      iostream(sb),
      local(address::unknown),
      remote(address::unknown),
      owner(_o), id(_id) { };
    virtual void close(void) = 0;
    // Unique identifier for this connection: used by connection manager
    int get_id() { return id ; };
    address &get_local(void) const { return local; };
    address &get_remote(void) const { return remote; };
    event_receiver& get_owner(void) const { return owner; };
  };

  class connection_event : public event {
  protected:
    // local endpoint for passive connections, remote endpoint for active
    // connections
    const address &t;
    connection_event(client_server &r, const address &_t) : event(r), t(_t) { };
  };

  class passive_connection_event: public connection_event {
  protected:
    passive_connection_event(server &r, const address &l):
      connection_event(r, l)
    {
    };
  public:
    static event *get(server &r, const address &l)
    {
      return new passive_connection_event(r, l);
    }
    const address &get_local(void) const { return t ; };
    virtual ~passive_connection_event() { };
  };

  class active_connection_event: public connection_event {
  protected:
    active_connection_event(client &r, const address &re):
      connection_event(r, re) { };
  public:
    static event *get(client &r, const address &re)
    {
      return new active_connection_event(r, re);
    }
    virtual ~active_connection_event() { };
  };

  class rx_event: public event {
  private:
    connection *c;
    client_server &cs;
  protected:
    rx_event(client_server& r, connection *_c) : event(r), c(_c), cs(r)
    {
    }
  public:
    virtual void doit(void);
    virtual ~rx_event() { };
    static event *get(client_server &r, connection *c)
    {
      return new rx_event(r, c);
    }
  };

} // namespace netpp
#endif // __connection_h__
