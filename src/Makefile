include $(BASE)/common.mk

OBJS :=

ifeq ($(DYNAMIC_LIB),y)
CXXLAGS += -fpic -fPIC
EXT := .so
else
EXT := .a
endif

ifeq ($(HOST),linux)
endif

ifeq ($(HOST),esp8266)
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
