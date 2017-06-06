#include <iostream>

#include "connection.h"
#include "host/server-ipv4-unix-socket.h"
#include "host/event-manager.h"
#include "logger.h"

using namespace std;
using namespace netpp;

class echo_server : public stream_server_ipv4_unix_socket {
public:
  echo_server(unsigned short port) : stream_server_ipv4_unix_socket(port) { };
  virtual void data_available(connection *);
  virtual void disconnected(connection *);
  virtual stream_server& operator << (connection *c);
};

void echo_server::data_available(connection *c)
{
  address r(c->get_remote());
  string ra; r.read(ra);
  log_info << "data available from " << ra << flush;
  string s;
  getline(*c, s);
  log_info << s << flush;
  *c << s << endl << flush;
}

void echo_server::disconnected(connection *c)
{
  address r(c->get_remote());
  string ra; r.read(ra);
  log_info << ra << " disconnected" << endl;
  stream_server::disconnected(c);
}

stream_server& echo_server::operator<<(connection *c)
{
  stream_server::operator<<(c);
  address r(c->get_remote());
  string ra; r.read(ra);
  log_info << " connection from " << ra << flush;
  return *this;
}

int main(int argc, char *argv[])
{
  echo_server s(3334);

  signal(SIGPIPE, SIG_IGN);
  while(linux_event_manager::get()->loop());
  log_info << "exiting";
}
