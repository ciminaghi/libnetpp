// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection.h: connection abstraction and relevant events

#include <memory>
#include "event.h"

class address;

class data_buf;

class server;

// This is the object creating connections
class passive_connection_factory {
public:
  passive_connection_factory() { };
  // Register a server for this connection factory
  passive_connection_factory & operator<< (server &);
  // To be called periodically
  virtual void handle(void) { };
  // This returns true in case the object is ready to produce a connection
  virtual bool poll(void) { return false; };
};

// This represents a connection with a remote peer
class connection {
private:
  std::shared_ptr<data_buf> rx_buf;
  const address &local, &remote;
protected:
  virtual bool rx_data_ready(void) = 0;
public:
  connection(const address &l, const address &r) : local(l), remote(r) { };
  // Returns true in case input data is ready
  virtual bool poll(void) { return false ; };
  // To be called periodically
  virtual void handle(void) { };
  virtual void abort(void) = 0;
  virtual void close(void) = 0;
  data_buf *get(void) { return rx_buf.get(); };
};

class connection_event : public event {
private:
  connection &c;
protected:
  connection &get_connection(void) const { return c ; };
public:
  connection_event(event_receiver &r, uint32_t mask, connection &_c) :
    event(r, mask), c(_c) { };
  connection_event(connection_event& ce): event(ce.get_receiver(),
						ce.get_compat_mask()),
					  c(ce.get_connection()) { };
};

class passive_connection_event: public connection_event {
public:
  passive_connection_event(event_receiver &r, connection &_c):
    connection_event(r, make_mask(GENERIC) | make_mask(SERVER), _c) { };
  passive_connection_event(const passive_connection_event& ce):
    connection_event(ce.get_receiver(), ce.get_compat_mask(),
		     ce.get_connection()) { };
  virtual void doit(void);
  virtual ~passive_connection_event() { };
};

class active_connection_event: public connection_event {
public:
  active_connection_event(event_receiver &r, connection &_c):
    connection_event(r, make_mask(GENERIC) | make_mask(CLIENT), _c) { };
  active_connection_event(const active_connection_event& ce):
    connection_event(ce.get_receiver(), ce.get_compat_mask(),
		     ce.get_connection()) { };
  virtual void doit(void);
  virtual ~active_connection_event() { };
};
