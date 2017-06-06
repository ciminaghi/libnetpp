// libnet: simple C++ networking library for arduino (and maybe not
// only arduino)
// LGPL v2.1
// Copyright Arduino S.r.l.
// Author Davide Ciminaghi 2017

// esp8266/host/netpp.h: low level compat header file


#include <stddef.h>
#include <stdio.h>

#define __cpp_sized_deallocation 0

extern "C" int snprintf(char *str, size_t size, const char *format, ...);
extern "C" int fileno(FILE *);
