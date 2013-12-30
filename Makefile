.PHONY: all clean debug release

all:
	$(MAKE) -C src

debug:
	$(MAKE) debug -C src

release:
	$(MAKE) release -C src

clean:
	$(MAKE) clean -C src


