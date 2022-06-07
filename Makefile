CFLAGS = -Wall -Wextra -std=c99 -I ./

.PHONY: lib_test clean

lib_test: bin/lib_test

bin/lib_test: build/lib_test.o build/lib.o
	@mkdir -p bin
	$(CC) -o $@ $^ -lcunit

build/lib_test.o: test/lib_test.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

build/lib.o: lib.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@if [ -d build ]; then rm -rf build; fi
	@if [ -d bin ];   then rm -rf bin;   fi
