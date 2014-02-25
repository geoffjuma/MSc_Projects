CC	= gcc
CFLAGS	= -O0 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example8.o

default: example8

.PHONY: default clean clobber

example8: $(objects)
	$(CC) -o $@ $^

example8.o: example8.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example8
