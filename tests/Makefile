all:: test

OBJS += $(patsubst %.c,%,$(wildcard *.c))

${OBJS}: %: %.c ../argparse.c
	$(CC) -I../ -o $@ ${DEP_LIBS} $^

test: ${OBJS}
	prove *.sh
.PHONY: test

clean:
	rm -f ${OBJS}
	find . -name '.o' -exec rm \;
.PHONY: clean
