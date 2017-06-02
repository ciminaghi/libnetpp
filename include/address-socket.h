// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// address-socket.h: socket addressing related classes
#ifndef __address_socket_h__
#define __address_socket_h__

#ifdef HAVE_LWIP
#include <sockets.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include "address.h"

namespace netpp {

  using namespace std;

  class address_socket : public address {
  private:
    struct sockaddr sa;
    int len;
    void do_set(void);
  public:
    static address_socket unknown;
    address_socket() : address(), sa{ PF_UNSPEC, } { }
    address_socket(const string& s) : address(s), sa { PF_UNSPEC, } { };
    address_socket(const struct sockaddr& _sa, socklen_t _len);
    void set(struct sockaddr &_sa, int _len)
    {
      sa = _sa; len = len;
      do_set();
    }
  };


} // namespace netapp

#endif // __address_socket_h__
