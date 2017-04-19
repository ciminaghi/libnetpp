
include $(BASE)/common.mk

ifeq ($(HOST),linux)

EXE := linux-simple-test

all: $(EXE)

linux-simple-test: % : %.o
	$(CC) -o $@ $+ $(LDFLAGS)
endif

ifeq ($(HOST),esp8266)

EXE := esp8266-simple-test

all: images

images: $(foreach e,$(EXE),do_$(e)_images)

do_%_images: %
	$(ESPTOOL) elf2image -o $*- $<

$(EXE): % : %.o
	$(LD) -L$(SDK_LIBDIR) $+ $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@

endif

$(eval $(call install_cmds,$(LIB),$(EXE),$(SCRIPTS)))

clean:
	rm -f $(EXE) *.o *~ *.bin *.out


.phony: all clean images $(foreach e,$(EXE),do_$(e)_images)
