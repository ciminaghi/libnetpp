// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// host/linux-event-manager.cpp: implementation of linux event manager

#include "host/event-manager.h"

namespace netpp {

  event_manager *event_manager::get(void)
  {
    if (!event_manager::instance)
      event_manager::instance = linux_event_manager::get();
    return instance;
  }

}
