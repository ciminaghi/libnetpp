// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include "event.h"

namespace netpp {

  // event.cpp: implementation of event related classes

  // event class implementation

  void event::arm(void)
  {
    *event_manager::get() << this;
  }

  // event_manager class implementation

  event_manager *event_manager::instance;

  event_manager::~event_manager()
  {
    // Consume all remaining events and die
    while (!active_events.empty()) {
      event *e = active_events.front();
      e->doit();
      active_events.pop_front();
    }
  }

  bool event_manager::loop(void)
  {
    while (!active_events.empty()) {
      event *e = active_events.front();
      active_events.pop_front();
      e->doit();
      if (e->single_shot() || e->to_be_deleted())
	delete e;
    }
    if (do_stop)
      return false;
    idle();
    return true;
  }

  event_manager & event_manager::operator << (event *e)
  {
    active_events.push_back(e);
    return *this;
  };

} // namespace netpp
