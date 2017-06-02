// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// logger : header file for logger class

#ifndef __logger_h__
#define __logger_h__

#include <streambuf>
#include <iostream>
#include <vector>

#include "timer.h"

using namespace std;
using namespace netpp;

namespace netpp {

  class logger_sb : public streambuf {
  private:
    char _buffer[80];
    string *prefix;
    virtual int_type overflow(int_type c);
    virtual int_type sync(void);
    int_type _flush(void);
  public:
    logger_sb(const char *p, size_t bufsize = 256);
    ~logger_sb() { sync(); };
  };

  class logger : public ostream {
  public:
    logger(streambuf *sb): ostream(sb) { };
  };

  extern class logger log_info, log_err, log_warn, log_fatal;

};

#endif // __logger_h__

