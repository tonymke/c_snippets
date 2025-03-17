.PHONY: all clean

SRC := hash.c hashtable.c prime_po2s.c

OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)

clean:
	$(RM) $(OBJ)

hash.o: hash.h
prime_po2s.o: prime_po2s.h
hashtable.o: hashtable.h prime_po2s.h
