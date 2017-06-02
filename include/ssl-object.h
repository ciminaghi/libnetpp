// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// ssl-object.h: ssl objects representing private keys, certificates, ....
#ifndef __ssl_object_h__
#define __ssl_object_h__

#include "logger.h"
#include <os_port.h>
#include <ssl.h>

using namespace std;
using namespace netpp;

namespace netpp {

  class ssl_object {
  public:
    enum type {
      UNKNOWN = -1,
      X509_CERT = SSL_OBJ_X509_CERT,
      X509_CACERT = SSL_OBJ_X509_CACERT,
      RSA_KEY = SSL_OBJ_RSA_KEY,
      PKCS8 = SSL_OBJ_PKCS8,
      PKCS12 = SSL_OBJ_PKCS12,
    };
  protected:
    type t;
    const uint8_t *data;
    int len;
    const string& password;
    virtual void map(void) = 0;
    virtual void unmap(void) { };
    static string default_password;
  public:
    ssl_object(type _t, const uint8_t *_data = nullptr, int _len = -1,
	       const string& _password = ssl_object::default_password):
      t(_t), data(_data), len(_len), password(_password)
    {
    }
    ssl_object(type _t = UNKNOWN, const string& _password =
	       ssl_object::default_password):
      t(_t), data(nullptr), len(-1), password(_password)
    {
    }
    void do_load(SSL_CTX *ctx)
    {
      if (data == nullptr)
	map();
      if (data == nullptr) {
	log_fatal << "cannot map ssl object" << endl << flush;
	return;
      }
      if (t == UNKNOWN) {
	log_fatal << "unknown type for ssl object" << endl << flush;
	return;
      }
      const char *pwd = password.size() ? password.c_str() : NULL;
      ssl_obj_memory_load(ctx, t, data, len, pwd);
    }
    virtual ~ssl_object()
    {
      if (data)
	unmap();
    };
  };

}; // namespace netpp

#endif // __ssl_object_h__
