CC=gcc
CFLAGS=
BINS=$(patsubst %.c, %, $(wildcard *.c))


default: $(BINS)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BINS)
