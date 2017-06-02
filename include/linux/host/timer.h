// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// linux timer class: linux system timer

#ifndef __linux_timer_h__
#define __linux_timer_h__

#include <signal.h>
#include <time.h>

#include <timer.h>

namespace netpp {

  class linux_timer : public timer {
  private:
    linux_timer() { };
  public:
    //static linux_timer *get(void)
    //{
    //linux_timer *out = static_cast<linux_timer *>(timer::get());
    //if (!out) {
    //out = new linux_timer();
    //instance = out;
    //}
    //return out;
    //}
    static linux_timer *get(void)
    {
      return new linux_timer();
    }
    virtual unsigned long curr_time(void)
    {
      struct timespec t;
      clock_gettime(CLOCK_MONOTONIC, &t);
      return t.tv_sec * 1000000 + t.tv_nsec * 1000;
    }
    virtual void schedule_tick(unsigned long when, event_receiver&)
    {
    }
    virtual bool get_next_evt_time(unsigned long long &)
    {
      return false;
    }
    virtual event *get_next_evt(void)
    {
      return NULL;
    }
  };

} // namespace netpp

#endif // __linux_timer_h__
