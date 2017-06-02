// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// address-ipv4.h: ipv4 address class
#ifndef __address_ipv4_h__
#define __address_ipv4_h__

#ifdef HAVE_LWIP
#include <sockets.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <string.h>

#include "address-socket.h"

namespace netpp {

  class address_ipv4 : public address_socket {
  private:
    struct in_addr a;
    unsigned short port;
  public:
    static address_ipv4 any;
    address_ipv4(const string& n);
    address_ipv4(const struct in_addr& _a):
      address_socket(inet_ntoa(a)), a(_a) { };
    address_ipv4(const in_addr_t _a) : address_socket(inet_ntoa(a)), a{_a,} { };
    address_ipv4(const in_addr_t _a, unsigned short port);
    address_ipv4(const struct sockaddr_in& _a):
      address_socket(inet_ntoa(a)), a{_a.sin_addr.s_addr} { };
    void read(struct in_addr& out) const { out = a; };
    void read(struct sockaddr_in& out) const
    { memset(&out, 0, sizeof(out));
      out.sin_family = AF_INET; out.sin_port = htons(port);
      out.sin_addr = a; };
    const struct in_addr &read(void) const { return a; };
  };

} // namespace netpp

#endif // __address_ipv4_h__
