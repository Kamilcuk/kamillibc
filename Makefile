B=build
all:
	cmake -H. -B$(B)
	$(MAKE) -C$(B)
test: all
	$(MAKE) -C$(B) test
clean:
	rm -vr $(B)
