// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection.cpp: implementation of connection related classes
#include <iostream>
#include "connection.h"

using namespace netpp;
using namespace std;


namespace netpp {

  // Implementation of rx_event

  void rx_event::doit(void)
  {
    if (c->peek() == EOF) {
      // connection was closed, warn the server
      cs.disconnected(c);
      return;
    }
    cs.data_available(c);
  }

};
