#include <string.h>
#include <streambuf>
#include "logger.h"
//#include "netpp.h"
//#include "address-socket.h"
#include "connection-axtls.h"
//#include "host/event-manager.h"

using namespace netpp;
using namespace std;

// connection-unix-socket.cpp: unix socket connection implementation

namespace netpp {

  // implementation of unix socket streambuf
  connection_axtls_sb::connection_axtls_sb(SSL *_ssl,
					   size_t bufsize,
					   size_t putback):
    ssl(_ssl),
    _put_back(max(putback, size_t(1))),
    _buffer(max(bufsize, _put_back) + _put_back)
  {
  }

  std::streambuf::int_type connection_axtls_sb::underflow(void)
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
    uint8_t *tmp;
    int n = ::ssl_read(ssl, &tmp);
    unsigned long sz, space;
    if (n > 0) {
      space = _buffer.size() - (unsigned long)(start - base);
      sz = space < (unsigned long)n ? space : n;
      ::memcpy(start, tmp, sz);
    }
    log_info << "ssl_read() returned " << n << endl;
    if (n < 0)
      return traits_type::eof();

    // Set buffer pointers
    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
  }

  streambuf::int_type connection_axtls_sb::uflow(void)
  {
    if (ssl_handshake_status(ssl) != SSL_OK)
      return *gptr();
    int_type n = underflow();
    if (n == EOF)
      return EOF;
    gbump(1);
    return gptr()[-1];
  }

  streambuf::int_type connection_axtls_sb::overflow(streambuf::int_type c)
  {
    if (c == EOF)
      return c;
    if (::ssl_write(ssl, (uint8_t *)&c, 1) < 0)
      return traits_type::eof();
    return c;
  }
}
