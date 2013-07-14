CFLAGS = -Wall -W -Wstrict-prototypes -Wwrite-strings -O3 -g -ggdb
LDFLAGS = -lm

LIBNAME=libargparse

DYLIBSUFFIX=so
STLIBSUFFIX=a
DYLIBNAME=$(LIBNAME).$(DYLIBSUFFIX)
DYLIB_MAKE_CMD=$(CC) -shared -o $(DYLIBNAME) $(LDFLAGS)
STLIBNAME=$(LIBNAME).$(STLIBSUFFIX)
STLIB_MAKE_CMD=ar rcs $(STLIBNAME)

# Platform-specific overrides
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),Darwin)
  DYLIBSUFFIX=dylib
  DYLIB_MAKE_CMD=$(CC) -shared -o $(DYLIBNAME) $(LDFLAGS)
endif

all: $(DYLIBNAME) $(STLIBNAME)

argparse.o: argparse.h

$(DYLIBNAME): argparse.o
	$(DYLIB_MAKE_CMD) $^

$(STLIBNAME): argparse.o
	$(STLIB_MAKE_CMD) $^

test: test_argparse
	@echo "###### Unit Test #####"
	@./test.sh

test_argparse: test_argparse.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf test_argparse
	rm -rf *.[ao]
