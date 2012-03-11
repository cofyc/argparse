CFLAGS = -std=c99 -Wall -g3

test: test_argparse
	@echo
	@echo "######Testing#####"
	@./test.sh

test_argparse: test.o argparse.o
	$(CC) $(CFLAGS) -o $@ $^

argparse.o: argparse.c
test.o: test.c

clean:
	rm -rf test
	rm -rf *.o
