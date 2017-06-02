// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// connection-unix-socket.h: unix socket connection abstraction and relevant
// events

#include "connection.h"
#include "address-socket.h"
#include "server.h"

using namespace std;

namespace netpp {

  class connection_unix_socket_sb : public streambuf {
  private:
    int fd;
    const size_t _put_back;
    vector<char> _buffer;
    // overrides base class underflow()
    int_type underflow();
    int_type overflow(int_type c);
  public:
    connection_unix_socket_sb(int _fd, size_t bufsize = 256,
			      size_t putback = 8);
    ~connection_unix_socket_sb() { }
  };

  class connection_unix_socket : public connection {
  private:
    address_socket &ls, &rs;
  public:
    connection_unix_socket(address_socket &l, address_socket &r,
			   int fd, client_server& _o);
    connection_unix_socket(address_socket &l, int fd, client_server& _o);
    connection_unix_socket(int fd, client_server& _o);
    virtual void close(void);
    virtual ~connection_unix_socket() { close(); };
  };

  class passive_connection_event_unix_socket : public passive_connection_event {
  protected:
    int fd;
    address_socket& local, remote;
    stream_server& srv;
  protected:
    passive_connection_event_unix_socket(stream_server&,
					 address_socket &l, int fd);
    virtual void doit(void);
    virtual connection *get_connection(int new_fd)
    {
      return new connection_unix_socket(local, remote, new_fd, srv);
    }
  public:
    static event *get(stream_server &r, address_socket& l, int fd);
    virtual ~passive_connection_event_unix_socket() { };
  };

} // namespace netpp
