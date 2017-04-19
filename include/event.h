// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// event.h: event abstraction

#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdint.h>

class event;
class connection;
class rx_buf;
class server;
class client;

// Introduce event receiver type to avoid rtti (embedded target !)
enum event_receiver_type {
  GENERIC = 0,
  SERVER = 1,
  CLIENT = 2,
};

static inline uint32_t make_mask(const event_receiver_type t)
{
  return (1 << t);
}

// All classes 
class event_receiver {
private:
  event_receiver_type t;
public:
  event_receiver(event_receiver_type _t = GENERIC): t(_t) { };
  event_receiver_type get_type(void) { return t; };
};

class event {
private:
  event_receiver &r;
  uint32_t receiver_compat_mask;
public:
  event(event_receiver &_r, uint32_t mask = make_mask(GENERIC)):
    r(_r), receiver_compat_mask(mask)
  {
    if (!(r.get_type() & mask))
      throw "Invalid receiver for this event";
  };
  event(event& e): r(e.get_receiver()) { };
  event_receiver& get_receiver(void) const { return r; };
  virtual void doit(void) { };
  uint32_t get_compat_mask(void) const { return receiver_compat_mask ; } ;
  ~event() { };
};


#if 0
class rx_event: public event {
private:
  rx_buf &b;
public:
  rx_event(event_receiver &r, rx_buf &_b): event(r), b(_b) { };
  
};

class 
#endif

#endif //__EVENT_H__
