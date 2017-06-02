// libnetpp: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017


// host-linux/ssl-object.cpp: implementation of ssl objects for the linux host
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "host/ssl-object.h"

// Just memory map the thing
int ssl_unix_file_object::length(void)
{
  struct stat buf;
  
  int ret = stat(path.c_str(), &buf);
  if (ret < 0) {
    log_fatal << "ssl_unix_file_object::length(): stat failed" << endl << flush;
    return -1;
  }
  return (int)buf.st_size;
}


void ssl_unix_file_object::map(void)
{
  int fd = ::open(path.c_str(), O_RDONLY);
  if (fd < 0) {
    log_fatal << "ssl_unix_file_object: cannot open file" << endl << flush;
    return;
  }
  len = length();
  if (len < 0)
    return;
  data = static_cast<uint8_t *>(::mmap(NULL, len,
				       PROT_READ, MAP_PRIVATE, fd, 0));
  ::close(fd);
  if (static_cast<const void *>(data) == MAP_FAILED) {
    log_fatal << "ssl_unix_file_object: cannot map file" << endl << flush;
    return;
  }
}

void ssl_unix_file_object::unmap(void)
{
  if (data)
    ::munmap(const_cast<uint8_t *>(data), len);
}
