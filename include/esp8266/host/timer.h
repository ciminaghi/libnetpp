// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// esp8266 timer class: esp8266 system timer

#ifndef __esp8266_timer_h__
#define __esp8266_timer_h__

#include <timer.h>
#include "user_interface.h"

namespace netpp {

  class esp8266_timer : public timer {
  private:
    esp8266_timer() { };
  public:
    static esp8266_timer *get(void)
    {
      return new esp8266_timer();
    }
    virtual unsigned long curr_time(void)
    {
      return ::system_get_time();
    }
    // To be implemented
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
