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
#include "host/connection-unix-socket.h"
#include "host/event-manager.h"

using namespace netpp;
using namespace std;

// connection-unix-socket.cpp: unix socket connection implementation

namespace netpp {

  // implementation of unix socket streambuf
  connection_unix_socket_sb::connection_unix_socket_sb(int _fd,
						       size_t bufsize,
						       size_t putback):
    fd(_fd),
    _put_back(max(putback, size_t(1))),
    _buffer(max(bufsize, _put_back) + _put_back)
  {
  }

  std::streambuf::int_type connection_unix_socket_sb::underflow(void)
  {
    if (gptr() < egptr()) // buffer not exhausted
        return traits_type::to_int_type(*gptr());

    char *base = &_buffer.front();
    char *start = base;

    if (eback() == base) // true when this isn't the first fill
    {
        // Make arrangements for putback characters
        ::memmove(base, egptr() - _put_back, _put_back);
        start += _put_back;
    }

    // start is now the start of the buffer, proper.
    // Read from fptr_ in to the provided buffer
    size_t n = ::read(fd, start, _buffer.size() - (start - base));
    if (n == 0)
        return traits_type::eof();

    // Set buffer pointers
    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
  }

  streambuf::int_type connection_unix_socket_sb::overflow(int_type c)
  {
    if (c == EOF)
      return c;
    if (::write(fd, &c, 1) < 0)
      return traits_type::eof();
    return c;
  }

  // implementation of unix socket connection class
  connection_unix_socket::connection_unix_socket(address_socket &l,
						 address_socket &r,
						 int fd,
						 client_server& _o) :
    connection(l, r, fd, _o, new connection_unix_socket_sb(fd)), ls(l), rs(r)
  {
    linux_event_manager::get()->add_fd(fd, rx_event::get(_o, this));
  };

  connection_unix_socket::connection_unix_socket(address_socket &l,
						 int fd,
						 client_server& _o) :
    connection(l, address_socket::unknown, fd, _o,
	       new connection_unix_socket_sb(fd)),
    ls(l),
    rs(address_socket::unknown)
  {
    linux_event_manager::get()->add_fd(fd, rx_event::get(_o, this));
  };

  connection_unix_socket::connection_unix_socket(int fd,
						 client_server& _o) :
    connection(address_socket::unknown, address_socket::unknown, fd,
	       _o, new connection_unix_socket_sb(fd)),
    ls(address_socket::unknown),
    rs(address_socket::unknown)
  {
    linux_event_manager::get()->add_fd(fd, rx_event::get(_o, this));
  };

  void connection_unix_socket::close(void)
  {
    linux_event_manager::get()->del_fd(get_id());
    ::close(get_id());
  }

  // implementation of passive_connection_event_unix_socket
  passive_connection_event_unix_socket::
  passive_connection_event_unix_socket(stream_server& r,
				       address_socket &l, int _fd):
    passive_connection_event(r, l), fd(_fd), local(l), srv(r) { };

  void passive_connection_event_unix_socket::doit(void)
  {
    struct sockaddr sa;
    socklen_t len = sizeof(sa);
    int new_fd = ::accept(fd, &sa, &len);
    if (new_fd < 0)
      fatal("accept error", 127);
    remote.set(sa, len);
    connection *newc = get_connection(new_fd);
    // Tell the server that a new connection has been established
    srv << newc;
  }

  event *passive_connection_event_unix_socket::get(stream_server &r,
						   address_socket& l,
						   int fd)
  {
    return new passive_connection_event_unix_socket(r, l, fd);
  }
  
} // namespace netpp
