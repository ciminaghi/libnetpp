// server-ipv4-lwip-raw.h: ipv4 servers via lwip raw api
#ifndef __server_ipv4_lwip_raw_h__
#define __server_ipv4_lwip_raw_h__

#include "server-ipv4.h"

//#include "lwip/opt.h"
//#include "lwip/debug.h"
//#include "lwip/stats.h"
//#include "lwip/tcp.h"

namespace netpp {

  class stream_server_ipv4_lwip_raw : public stream_server_ipv4 {
  private:
    struct tcp_pcb *tcb;
  public:
    stream_server_ipv4_lwip_raw(address_ipv4 &local);
    stream_server_ipv4_lwip_raw(unsigned short port);
    virtual ~stream_server_ipv4_lwip_raw() { };
  };

  class dgram_server_ipv4_lwip_raw : public dgram_server_ipv4 {
  private:
    struct udp_pcb *tcb;
  public:
    dgram_server_ipv4_lwip_raw(address_ipv4 &local);
    dgram_server_ipv4_lwip_raw(unsigned short port);
    virtual ~dgram_server_ipv4_lwip_raw() { };
  };

} // namespace netpp

#endif // __server_ipv4_lwip_raw_h__
