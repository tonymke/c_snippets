.PHONY: all ansi_c clean

all: ansi_c

ansi_c:
	$(MAKE) -C ansi_c all

clean:
	$(MAKE) -C ansi_c clean

ansi_c/%:
	$(MAKE) -C ansi_c $(patsubst ansi_c/%,%,$@)
