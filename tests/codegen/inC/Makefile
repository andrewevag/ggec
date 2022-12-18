CC=clang
CFLAGS=
BINS=bsort hanoi mean primes strrev 

default: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BINS)