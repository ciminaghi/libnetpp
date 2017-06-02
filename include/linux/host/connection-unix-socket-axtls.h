// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection-unix-socket.h: unix socket connection abstraction and relevant
// events

#include "connection-unix-socket.h"
#include "address-ipv4.h"
#include "host/event-manager.h"
#include <ssl.h>

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
    int_type overflow(int_type c);
  public:
    connection_axtls_sb(SSL *ssl, size_t bufsize = 256, size_t putback = 8);
    ~connection_axtls_sb() { ssl_free(ssl); }
  };

  class connection_unix_socket_axtls : public connection {
  public:
    connection_unix_socket_axtls(address &l, address &r, int id,
				 client_server& _o, SSL *ssl) :
      connection(l, r, id, _o, new connection_axtls_sb(ssl))
    {
      linux_event_manager::get()->add_fd(id, rx_event::get(_o, this));
    };
    connection_unix_socket_axtls(address &l, int _id, client_server& _o,
				 SSL *ssl):
      connection(l, address::unknown, _id, _o, new connection_axtls_sb(ssl))
    {
      linux_event_manager::get()->add_fd(_id, rx_event::get(_o, this));
    };
    connection_unix_socket_axtls(int _id, client_server& _o, SSL *ssl):
      connection(address_ipv4::any, address::unknown, _id, _o,
		 new connection_axtls_sb(ssl))
    {
      linux_event_manager::get()->add_fd(_id, rx_event::get(_o, this));
    };
    virtual void close(void) { }
    ~connection_unix_socket_axtls() { close(); };
  };

  class secure_passive_connection_event_unix_socket :
    public passive_connection_event_unix_socket {
  private:
    SSL_CTX *ctx;
    SSL *ssl;
  protected:
    secure_passive_connection_event_unix_socket(stream_server& s,
						address_socket &l, int fd,
						SSL_CTX *_ctx):
      passive_connection_event_unix_socket(s, l, fd), ctx(_ctx),
      ssl(nullptr) { };
    virtual connection *get_connection(int new_fd)
    {
	ssl = ssl_server_new(ctx, new_fd);
	return new connection_unix_socket_axtls(local, remote, new_fd,
						srv, ssl);
    }
  public:
    static event *get(stream_server &, address_socket&, int fd, SSL_CTX *);
    virtual ~secure_passive_connection_event_unix_socket()
    {
      if (ssl)
	ssl_free(ssl);
    };
  };

} // namespace netpp
