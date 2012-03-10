CFLAGS = -std=c99 -Wall -g3

test: test.o argparse.o
	$(CC) $(CFLAGS) -o $@ $^

argparse.o: argparse.c
test.o: test.c

clean:
	rm -rf test
	rm -rf *.o
