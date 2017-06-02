// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <streambuf>
#include "netpp.h"
#include "address-socket.h"
#include "host/connection-unix-socket-axtls.h"
#include "host/event-manager.h"

using namespace netpp;
using namespace std;

// connection-unix-socket-axtls.cpp: secure (axtls) unix socket connection
// implementation

namespace netpp {

  // event for secure passive connections


  event *secure_passive_connection_event_unix_socket::get(stream_server &r,
							  address_socket& l,
							  int fd,
							  SSL_CTX *_ctx)
  {
    return new secure_passive_connection_event_unix_socket(r, l, fd, _ctx);
  }
  
} // namespace netpp
