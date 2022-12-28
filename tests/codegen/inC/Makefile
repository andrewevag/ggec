CC=gcc
CFLAGS=-std=gnu11
BINS=$(patsubst %.c, %, $(wildcard *.c))


default: $(BINS)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BINS)
