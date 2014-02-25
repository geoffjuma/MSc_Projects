CC	= gcc
CFLAGS	= -O2 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example4.o

default: example4

.PHONY: default clean clobber

example4: $(objects)
	$(CC) -o $@ $^

example4.o: example4.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example4
