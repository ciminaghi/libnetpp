#include <iostream>

#include "connection.h"
#include "host/client-ipv4-unix-socket.h"
#include "host/event-manager.h"
#include "logger.h"

using namespace std;
using namespace netpp;

namespace netpp {

  class echo_client : public stream_client_ipv4_unix_socket {
  public:
    echo_client(address_ipv4 &remote) :
      stream_client_ipv4_unix_socket(remote) { };
    virtual void data_available(connection *);
    virtual void disconnected(connection *);
  };

  class event_stdin_ready : public event {
  private:
    echo_client &cl;
  protected:
    event_stdin_ready(echo_client& r): event(r), cl(r) { };
  public:
    static event_stdin_ready *get(echo_client& r)
    { return new event_stdin_ready(r); };
    virtual void doit(void)
    {
      string s;
      cin >> s;
      log_info << "sending data" << endl;
      *(cl.get_connection()) << s << endl << flush;
    };
    virtual ~event_stdin_ready() { };
  };


  void echo_client::data_available(connection *c)
  {
    address r(c->get_remote());
    string ra; r.read(ra);
    log_info << " data from " << ra << flush;
    string s;
    *c >> s;
    log_info << s << flush;
  }

  void echo_client::disconnected(connection *c)
  {
    address r(c->get_remote());
    string ra; r.read(ra);
    log_info << ra << " disconnected" << endl;
    *event_manager::get() << fatal_event::get_instance();
  }
};

int main(int argc, char *argv[])
{
  event_stdin_ready *e;

  if (argc < 2) {
    cerr << "Use " << argv[0] << "ip:port" << endl;
    exit(127);
  }
  address_ipv4 a(argv[1]);
  echo_client c(a);

  // Add event from stdin (triggered every time stdin is ready)
  e = event_stdin_ready::get(c);
  linux_event_manager::get()->add_fd(0, e);
  signal(SIGPIPE, SIG_IGN);
  while(linux_event_manager::get()->loop());
  log_info << "exiting";
}
