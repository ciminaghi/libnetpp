// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection-ssl.h: ssl encrypted connection abstraction and relevant
// events

#include <ssl.h>
// FIXME ?
#undef min
#undef max

#include "connection.h"
#include "address-ipv4.h"
#include "server.h"

using namespace std;
using namespace netpp;

namespace netpp {

  class connection_axtls_sb : public streambuf {
  private:
    SSL *ssl;
    const size_t _put_back;
    vector<char> _buffer;
    // overrides base class underflow()
    int_type underflow();
    int_type uflow();
    int_type overflow(int_type c);
  public:
    connection_axtls_sb(SSL *ssl, size_t bufsize = 256, size_t putback = 8);
    ~connection_axtls_sb() { }
  };

  class connection_axtls : public connection {
  public:
    connection_axtls(address &l, address &r, int id,
		     client_server& _o, SSL *ssl) :
      connection(l, r, id, _o, new connection_axtls_sb(ssl)) { };
    connection_axtls(address &l, int _id, client_server& _o, SSL *ssl):
      connection(l, address::unknown, _id, _o, new connection_axtls_sb(ssl))
    { };
    connection_axtls(int _id, client_server& _o, SSL *ssl):
      connection(address_ipv4::any, address::unknown, _id, _o,
		 new connection_axtls_sb(ssl))
    { };
    virtual void close(void);
    ~connection_axtls() { };
  };
};
