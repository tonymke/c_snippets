.PHONY: all clean

CFLAGS := $(patsubst -std=%,-std=c90,$(CFLAGS))

SRC := hash.c hashtable.c logging.c prime_po2s.c string_utils.c

OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)

clean:
	$(RM) $(OBJ) $(BIN)

hash.o: hash.h
hashtable.o: hashtable.h prime_po2s.h
logging.o: logging.h string_utils.h
prime_po2s.o: prime_po2s.h
string_utils.o: string_utils.h
