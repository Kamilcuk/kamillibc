SRCS=$(shell find . -type f -name '*.c') $(shell find ../src -type f -name '*.c')
OBJS=$(patsubst %.c,%.o,$(SRCS))
MAIN=test.out

CFLAGS=-O3 -Wall -I../src

all: test.out

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

test.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm

clean:
	-rm -v $(OBJS)
	-rm -v test.out
