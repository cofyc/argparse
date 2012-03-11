CFLAGS = -std=c99 -Wall -g3

test: test_argparse
	@echo
	@echo "######Testing#####"
	@./test.sh

test_argparse: test_argparse.o argparse.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf test_argparse
	rm -rf *.o
