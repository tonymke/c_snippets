.PHONY: all clean

CFLAGS ?= -Og -g -pedantic -std=c90 -Wall -Werror -Wextra -Wfatal-errors \
	-Wno-error=pedantic -Wno-error=unused-parameter

SRC := hash.c prime_po2s.c

OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)

clean:
	$(RM) $(OBJ)

hash.o: hash.h
prime_po2s.o: prime_po2s.h
