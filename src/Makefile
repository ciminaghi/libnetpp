include $(BASE)/common.mk

OBJS := event.o connection.o server.o logger.o

ifeq ($(HAVE_AXTLS),y)
OBJS += connection-axtls.o
endif

CFLAGS += -g

ifeq ($(DYNAMIC_LIB),y)
CXXLAGS += -fpic -fPIC
EXT := .so
else
EXT := .a
endif

ifeq ($(HOST),linux)
OBJS += host-linux/event-manager.o host-linux/server-ipv4-unix-socket.o \
	host-linux/connection-unix-socket.o host-linux/address-socket.o \
	host-linux/address-ipv4.o host-linux/client-ipv4-unix-socket.o \
	host-linux/timer.o

ifeq ($(HAVE_AXTLS),y)
OBJS += host-linux/connection-unix-socket-axtls.o \
	host-linux/server-ipv4-unix-socket-axtls.o \
	host-linux/ssl-object.o
endif
endif

ifeq ($(HOST),esp8266)
OBJS += host-esp8266/timer.o
endif

ifeq ($(HAVE_LWIP),y)
CXXFLAGS += -DHAVE_LWIP
endif

OBJS +=

LIB := libnet$(EXT)

all: $(LIB)

$(eval $(call install_cmds,$(LIB),$(EXECUTABLES),$(SCRIPTS)))

clean:
	find . -name \*.o -or -name \*~ | xargs rm -f
	rm -f $(LIB) $(EXECUTABLES)
	rm -f url-tab.c


libnet.a: $(OBJS)
	$(AR) crus $@ $^

libnet.so: $(OBJS)
	$(CC) -o $@ -shared -Wl,-soname,$@ $^

.PHONY: install all clean
