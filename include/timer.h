// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// timer class: system timer abstraction

#ifndef __timer_h__
#define __timer_h__

#include "event.h"

namespace netpp {

  class timer {
  protected:
    static timer *instance;
    timer() { };
  public:
    static timer *get();
    // Return current time in usecs
    virtual unsigned long curr_time(void) = 0;
    // Schedule a timer tick to be sent on <when> usecs from now
    virtual void schedule_tick(unsigned long when, event_receiver&) = 0;
    // Returns true if there's at least a scheduled event
    virtual bool get_next_evt_time(unsigned long long &) = 0;
    virtual event *get_next_evt(void) = 0;
    virtual ~timer() { };
  };

  class timer_event : public event {
  public:
    timer_event(event_receiver& r);
    virtual void doit();
  };


} // namespace netpp

#include "host/timer.h"

#endif // __timer_h__
