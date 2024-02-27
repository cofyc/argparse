all: test 

test: subcommands basic
	python3 -m pytest $(shell pwd)
.PHONY: test

subcommands: subcommands.c ../argparse.c
	$(CC) -g -Werror -I../ -o subcommands $^

basic: basic.c ../argparse.c
	$(CC) -I../ -o basic $^

clean:
	rm -f subcommands basic
	find . -name '.o' -exec rm \;
.PHONY: clean
