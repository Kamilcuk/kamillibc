all:
	gcc -std=gnu17 -g3 -O3 -x c -o /tmp/a.out -Wall -Wextra -pedantic -lpthread src/staging/async/async.c src/staging/async/async_unittest.c
	#timeout 3 /tmp/a.out
	timeout 5 valgrind --leak-check=full /tmp/a.out 