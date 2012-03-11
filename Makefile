CFLAGS = -std=c99 -Wall -g3

test: test_argparse
	@echo
	@echo "###### Unit Test #####"
	@./test.sh

OBJS += argparse.o
OBJS += test_argparse.o

$(OBJS): argparse.h

test_argparse: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf test_argparse
	rm -rf *.o
