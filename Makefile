.PHONY: all src clean

CFLAGS ?= -Og -g -pedantic -Wall -Werror -Wextra -Wfatal-errors \
	-Wno-error=pedantic -Wno-error=unused-parameter

all: src

clean:
	$(MAKE) -C src clean

src:
	$(MAKE) -C src all
