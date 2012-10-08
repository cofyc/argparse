CFLAGS = -Wall -g3
LDFLAGS = -lm

all: test_argparse

test: test_argparse
	@echo
	@echo "###### Unit Test #####"
	@./test.sh

OBJS += argparse.o
OBJS += test_argparse.o

$(OBJS): argparse.h

test_argparse: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf test_argparse
	rm -rf *.o
