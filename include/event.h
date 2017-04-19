// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// event.h: event abstraction

#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include <deque>
#include <list>

using namespace std;

namespace netpp {

  class event;
  class event_manager;
  class connection;
  class rx_buf;
  class server;
  class client;

  // All objects which can receive events are event receivers
  class event_receiver {
  public:
    event_receiver() { };
  };

  class event {
  private:
    bool _to_be_deleted;
  protected:
    event_receiver &r;
    event(event_receiver &_r): _to_be_deleted(false), r(_r)
    {
    };
  public:
    event_receiver& get_receiver(void) const { return r; };
    virtual void doit(void) { };
    virtual ~event() { };
    static event *get_instance(event_receiver &r)
    {
      return new event(r);
    }
    virtual void arm(void);
    virtual bool single_shot(void) { return false; }
    void mark_for_deletion(void) { _to_be_deleted = true; };
    bool to_be_deleted(void) { return _to_be_deleted; };
  };

  // This implements the idle loop, which handles events. It is a singleton.
  class event_manager : public event_receiver {
  private:
    bool do_stop;
  protected:
    static event_manager *instance;
    deque<event *> active_events;
    // This will be reimplemented on actual hosts
    virtual void idle() { };
    event_manager(): do_stop(false) { };
    virtual ~event_manager();
  public:
    static event_manager *get(void);
    // This can optionally be reimplemented. Returns false when we have to
    // exit
    virtual bool loop();
    virtual void stopit(void) { do_stop = true ; };
    event_manager & operator << (event *e);
  };
  

  class fatal_event : public event {
  protected:
    fatal_event(): event(*event_manager::get())
    {
    };
  public:
    static event *get_instance(void)
    {
      return new fatal_event();
    };
    virtual void doit(void)
    {
      event_manager::get()->stopit();
    }
    virtual bool single_shot(void) { return true; }
    virtual ~fatal_event() { };
  };

} // namespace netpp

#endif //__EVENT_H__
