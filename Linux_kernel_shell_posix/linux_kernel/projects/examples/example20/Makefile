CC	= gcc
CFLAGS	= -O0 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example20.o

default: example20

.PHONY: default clean clobber

example20: $(objects)
	$(CC) -o $@ $^

example20.o: example20.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example20
