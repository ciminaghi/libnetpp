// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

#include <string>
#include <iostream>
#include <sstream>

#include "address-ipv4.h"

using namespace std;
using namespace netpp;

namespace netpp {

  address_ipv4 address_ipv4::any(INADDR_ANY);

  address_ipv4::address_ipv4(const string& n) : address_socket(n)
  {
    size_t pos;
    string sub(n);
    if ((pos = n.find(':')) != string::npos) {
      // Address and port
      sub = n.substr(0, pos);
      inet_aton(sub.c_str(), &a);
      string sub2 = n.substr(pos + 1, n.size());
      port = atoi(sub2.c_str());
    }
    set_name(sub);
  }

  address_ipv4::address_ipv4(const in_addr_t _a, unsigned short port):
    address_socket()
  {
    struct in_addr __a{_a};
    string n = inet_ntoa(__a);
    stringstream ss(n);
    ss << ":" << port;
    set_name(ss.str());
  }

}
