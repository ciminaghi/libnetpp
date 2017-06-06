// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// netpp main header
#ifndef __netpp_h__
#define __netpp_h__

#include "host/netpp.h"
#include "logger.h"
#include "event.h"

namespace netpp {

  static inline void fatal(const string& s, int code)
  {
    log_fatal << s << endl << flush;
    *event_manager::get() << fatal_event::get_instance();
  }

};

#endif // __netpp_h__
