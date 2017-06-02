// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "host/server-ipv4-unix-socket-axtls.h"
#include "ssl-object.h"
#include "host/event-manager.h"


using namespace netpp;
using namespace std;

namespace netpp {

  stream_server_ipv4_unix_socket_axtls::
  stream_server_ipv4_unix_socket_axtls(address_ipv4 &local,
				       vector<ssl_object *>& objs):
    stream_server_ipv4_unix_socket(local, true),
    ssl_ctx(ssl_ctx_new(/* SSL_CLIENT_AUTHENTICATION |*/
			SSL_DISPLAY_CERTS |
			SSL_DISPLAY_BYTES|SSL_DISPLAY_STATES,
			SSL_DEFAULT_SVR_SESS))
  {
    // Create event
    e = secure_passive_connection_event_unix_socket::get(*this,
							 local,
							 fd,
							 ssl_ctx);
    linux_event_manager *em = linux_event_manager::get();
    em->add_fd(fd, e);
    for (vector<ssl_object *>::iterator i = objs.begin();
	 i != objs.end(); ++i)
      (*i)->do_load(ssl_ctx);
  }

  stream_server_ipv4_unix_socket_axtls::
  stream_server_ipv4_unix_socket_axtls(unsigned short port,
				       vector<ssl_object *>& objs):
    stream_server_ipv4_unix_socket(port, true),
    ssl_ctx(ssl_ctx_new(/* SSL_CLIENT_AUTHENTICATION, */
			SSL_DISPLAY_CERTS
			/*SSL_DISPLAY_BYTES|SSL_DISPLAY_STATES*/,
			SSL_DEFAULT_SVR_SESS))
  {
    // Create event
    e = secure_passive_connection_event_unix_socket::get(*this,
							 local_ipv4,
							 fd,
							 ssl_ctx);
    linux_event_manager *em = linux_event_manager::get();
    em->add_fd(fd, e);
    for (vector<ssl_object *>::iterator i = objs.begin();
	 i != objs.end(); ++i)
      (*i)->do_load(ssl_ctx);
  }

  stream_server_ipv4_unix_socket_axtls::~stream_server_ipv4_unix_socket_axtls()
  {
    linux_event_manager *em = linux_event_manager::get();
    em->del_fd(fd);
    close(fd);
  }

} // namespace netpp
