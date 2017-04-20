// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// address.h: addressing related classes
#ifndef __address_h__
#define __address_h__

#include <string>
using namespace std;

namespace netpp {

  // Simple address, can be ipv4, ipv6, path in the filesystem, .....
  class address {
  private:
    string name;
  protected:
    void set_name(const string& n) { name = n; };
  public:
    static address unknown;
    address(): name("UNSPECIFIED") { };
    address(const string& n): name(n) { };
    address(const address& a) : name(a.read()) { };
    void read(string& o) const { o = name; };
    const string &read(void) const { return name ; };
  };
} // namespace netpp

#endif // __address_h__
