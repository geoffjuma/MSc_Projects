CC	= gcc
CFLAGS	= -O0 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example7.o

default: example7

.PHONY: default clean clobber

example7: $(objects)
	$(CC) -o $@ $^

example7.o: example7.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example7
