// libnetpp: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// linux/ssl-object.h: ssl objects representing private keys, certificates, ....
// Linux/unix specialized classes

#ifndef __linux_ssl_object_h__
#define __linux_ssl_object_h__

#include <ssl-object.h>

using namespace std;
using namespace netpp;

namespace netpp {

  class ssl_unix_file_object : public ssl_object {
  protected:
    const string& path;
    int length(void);
    virtual void map(void);
    virtual void unmap(void);
  public:
    ssl_unix_file_object(type t, const string& _path,
			 const string& _password =
			 ssl_object::default_password) :
      ssl_object(t, _password), path(_path) { };
    virtual ~ssl_unix_file_object() { };
  };

  class ssl_cert_unix_file_object : public ssl_unix_file_object {
  public:
    ssl_cert_unix_file_object(const string& _path,
			      const string& _password = ""):
      ssl_unix_file_object(X509_CERT, _path, _password)
    {
    }
    virtual ~ssl_cert_unix_file_object() { };
  };

  class ssl_ca_cert_unix_file_object : public ssl_unix_file_object {
    ssl_ca_cert_unix_file_object(const string& _path,
				 const string& _password = ""):
      ssl_unix_file_object(X509_CACERT, _path, _password)
    {
    }
    virtual ~ssl_ca_cert_unix_file_object() { };
  };

  class ssl_private_key_unix_file_object: public ssl_unix_file_object {
    ssl_private_key_unix_file_object(const string& _path,
				     const string& _password = ""):
      ssl_unix_file_object(UNKNOWN, _path, _password)
    {
      if (_path.find(".p8"))
	t = PKCS8;
      else if (_path.find(".p12"))
	t = PKCS12;
      else
	t = RSA_KEY;
    }

  };
  
}; // namespace netpp

#endif // __linux_ssl_object_h__
