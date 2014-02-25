CC	= gcc
CFLAGS	= -O2 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example11.o

default: example11

.PHONY: default clean clobber

example11: $(objects)
	$(CC) -o $@ $^

example11.o: example11.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example11
