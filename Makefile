CC      = clang
CFLAGS  = -Wall -Wextra -g -O0 -IInclude
SAN     = -fsanitize=address,undefined

build/vec: build/Test.o build/Vec.o
	$(CC) $(CFLAGS) -o $@ $^

build/Test.o: Tests/Test.c Include/Vec.h
	$(CC) $(CFLAGS) -c Tests/Test.c -o $@

build/Vec.o: Src/Vec.c Include/Vec.h
	$(CC) $(CFLAGS) -c Src/Vec.c -o $@

.PHONY: clean
clean:
	rm -f build/*.o build/vec
