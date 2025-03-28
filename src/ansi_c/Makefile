.PHONY: all clean

CFLAGS := $(patsubst -std=%,-std=c90,$(CFLAGS))

SRC := hash.c hashtable.c log.c prime_po2s.c str.c

OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)

clean:
	$(RM) $(OBJ) $(BIN)

hash.o: hash.h
hashtable.o: hashtable.h prime_po2s.h
log.o: log.h str.h
prime_po2s.o: prime_po2s.h
str.o: str.h
