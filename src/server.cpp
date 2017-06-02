// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// server.cpp: server implementation

#include "address.h"
#include "server.h"

using namespace std;
using namespace netpp;

namespace netpp {

  void stream_server::disconnected(connection *c)
  {
    remove_connection(c);
    delete(c);
  }

};
