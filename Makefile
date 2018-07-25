CC=gcc -std=gnu17 -O3 -flto -x c -Wall -Wextra -pedantic -lpthread
all:
	$(CC) -o /tmp/a.out src/staging/async/async.c src/staging/async/async_unittest.c
	# timeout 3 /tmp/a.out
	timeout 10 bash -c 'time valgrind --leak-check=full /tmp/a.out' 
	$(CC) -DNDEBUG -o /tmp/a.out src/staging/async/async.c src/staging/async/async_unittest.c
	# timeout 3 /tmp/a.out
	timeout 10 bash -c 'time valgrind --leak-check=full /tmp/a.out' 