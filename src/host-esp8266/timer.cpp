// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// timer class: system timer implementation for Linux host (actually any posix)

#include "host/timer.h"

using namespace netpp;

namespace netpp {

  timer *timer::instance;
  
  timer *timer::get(void)
  {
    if (!instance)
      instance = esp8266_timer::get();
    return instance;
  }


};
