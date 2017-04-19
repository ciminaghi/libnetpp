

include $(BASE)/common.mk.$(HOST)


# Toolchain
HOSTCC ?= gcc
HOSTLD ?= ld
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip
INSTALL ?= /usr/bin/install
DEBUG ?= n
ESPTOOL ?= /home/develop/yun/esp8266/esp-open-sdk/esptool/esptool.py

# Internal libraries
LIBNET := -L$(BASE)/src -lnet

prefix := /usr
datadir := $(prefix)/share/dfu/data
confdir := /etc

libdir := $(prefix)/lib/dfu
bindir := $(prefix)/bin

INSTALL_DEFAULT_OWNER_GROUP := --owner=root --group=root
INSTALL_DEFAULT_MODE :=
#INSTALL_DEFAULT_STRIP := --strip --strip-program=$(STRIP)


# Common definitions for cflags and ldflags
# -DPREFIX=\"$(prefix)/\"
CXXFLAGS += -std=c++11 -O2 -Wall -Werror -DHOST_$(HOST) -I$(BASE)/include/ -I. \
	-DBASEDIR=\"$(BASE)\" \
	-DBINDIR=\"$(bindir)/\" -DLIBDIR=\"$(libdir)/\" \
	-DDATADIR=\"$(datadir)/\" -DCONFDIR=\"$(confdir)/\" \
	-I$(BASE)/include/$(HOST)
LDFLAGS +=  $(LIBNET) -lstdc++

ifeq ($(DEBUG),y)
CXXFLAGS += -g -DDEBUG
endif

CXXFLAGS += $(EXTRA_CXXFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)

# Common target defines
define install_cmds
install: all lib_install bin_install

$(eval ifneq (,$(strip $(1)))
lib_install: libdir_install $(1)
	$(INSTALL) $(INSTALL_DEFAULT_OWNER_GROUP) $(INSTALL_DEFAULT_STRIP) \
	$(INSTALL_DEFAULT_MODE) $(foreach l,$(1),$l) $(DESTDIR)/$(libdir)
else
lib_install:
endif)

$(eval ifneq (,$(strip $(2)))
bin_install: bindir_install $(2)
	$(INSTALL) $(INSTALL_DEFAULT_OWNER_GROUP) $(INSTALL_DEFAULT_STRIP) \
	$(INSTALL_DEFAULT_MODE) $(foreach l,$(2),$l) $(DESTDIR)/$(bindir)
	[ -n "$(foreach l,$(3),$l)" ] && \
	$(INSTALL) $(INSTALL_DEFAULT_OWNER_GROUP) \
	$(INSTALL_DEFAULT_MODE) $(foreach l,$(3),$l) $(DESTDIR)/$(bindir) \
	|| exit 0
else
bin_install:
endif)

$(eval bindir_install libdir_install: %_install:
	$(INSTALL) -d $(DESTDIR)/$$($$*) $(INSTALL_DEFAULT_OWNER_GROUP) \
	$(INSTALL_DEFAULT_MODE))
endef
