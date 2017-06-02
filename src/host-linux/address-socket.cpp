// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// address-socket.cpp: implementation of socket addressing related classes


#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>

#include "address-socket.h"

using namespace netpp;
using namespace std;

address_socket address_socket::unknown;

void address_socket::do_set(void)
{
  switch (sa.sa_family) {
  case AF_INET:
    {
      struct sockaddr_in sai;
      stringstream ss;
      memcpy(&sai, (sockaddr_in *)&sa, sizeof(sai));
      ss << inet_ntoa(sai.sin_addr) << ":" << sai.sin_port;
      set_name(ss.str());
      break;
    }
    // FIXME: IMPLEMENT AF_INET6
    // ....
  case AF_UNIX:
    {
      struct sockaddr_un sau;
      memcpy(&sau, (sockaddr_un *)&sa, sizeof(sau));
      set_name(sau.sun_path);
      break;
    }
  default:
    break;
  }
}

address_socket::address_socket(const struct sockaddr& _sa, socklen_t _len) :
  address(), sa(_sa), len(_len)
{
  do_set();
}
