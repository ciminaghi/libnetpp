// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// logger: logger class implementation

#include <cstdio>
#include "netpp.h"
#include "logger.h"
#include "timer.h"

using namespace std;

namespace netpp {
  logger log_info(new logger_sb("INFO"));
  logger log_err(new logger_sb("ERROR"));
  logger log_warn(new logger_sb("WARN"));
  logger log_fatal(new logger_sb("FATAL"));

  logger_sb::logger_sb(const char *p, size_t bufsize) : prefix(new string(p))
  {
    setp (_buffer, _buffer + sizeof(_buffer) - 1);
  };

  ostream::int_type logger_sb::_flush(void)
  {
    ostream::int_type n = pptr() - pbase();
    ::write(1, _buffer, n);
    pbump(-n);
    return n;
  }

  ostream::int_type logger_sb::overflow(int_type c)
  {
    if (c == EOF)
      return c;
    *pptr() = c;
    pbump(1);
    _flush();
    return c;
  }

  int logger_sb::sync(void)
  {
    cout << endl << "[ " << *prefix << ": " << timer::get()->curr_time()
	 << "] " << flush;
    return _flush();
  }
  
};
