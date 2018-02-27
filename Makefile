
all:
	$(MAKE) m4.Makefile
	$(MAKE) test

./test/test.out:
	$(MAKE) -C ./test

test: ./test/test.out
	./test/test.out

clean:
	$(MAKE) -C ./test clean

.PHONY: test ./test/test.out

