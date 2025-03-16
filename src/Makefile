.PHONY: all ansi_c clean

CFLAGS := $(subst -std=c99,-std=c90,$(CFLAGS))

all: ansi_c

ansi_c:
	$(MAKE) -C ansi_c all

clean:
	$(MAKE) -C ansi_c clean
