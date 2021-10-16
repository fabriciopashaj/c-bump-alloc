CFLAGS = -Wall -Wextra -std=c99 -I ./

bin/lib_test: test/lib_test.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ -lcunit
