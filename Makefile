.PHONY: all src clean

export CFLAGS ?= -Og -g -pedantic -std=c99 -Wall -Werror -Wextra \
		 -Wfatal-errors -Wno-error=pedantic -Wno-error=unused-parameter
export LDFLAGS ?=
export LDLIBS ?=

all: src

clean:
	$(MAKE) -C src clean

src:
	$(MAKE) -C src all
