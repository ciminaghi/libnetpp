// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// server.h: server abstraction and relevant events
#ifndef __server_h__
#define __server_h__

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "client-server.h"
#include "connection.h"

using namespace std;

namespace netpp {

  // Base stream class is declared in client-sever.h

  class stream_server : public server {
  private:
    vector<connection *> connections;
  private:
    stream_server& remove_connection(connection *c)
    {
      vector<connection *>::iterator i;
      i = find(connections.begin(), connections.end(), c);
      if (i != connections.end())
	connections.erase(i);
      return *this;
    }
  public:
    stream_server(const address &local) : server(local) { };
    virtual stream_server& operator << (connection *c)
    {
      connections.push_back(c);
      return *this;
    }
    virtual stream_server& operator >> (connection *c)
    {
      return remove_connection(c);
    }
    virtual void disconnected(connection *c);
    virtual bool is_stream(void) { return true ; };
    virtual ~stream_server() { };
  };

  class dgram_server : public server {
  public:
    dgram_server(const address &local) : server(local) { };
    virtual void data_available(connection *c);
    virtual bool is_stream(void) { return false ; };
    virtual ~dgram_server() { };
  };

} // namespace netpp

#endif // __server_h__
