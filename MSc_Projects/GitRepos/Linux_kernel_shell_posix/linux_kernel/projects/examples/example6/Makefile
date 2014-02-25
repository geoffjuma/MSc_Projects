CC	= gcc
CFLAGS	= -O0 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example6.o

default: example6

.PHONY: default clean clobber

example6: $(objects)
	$(CC) -o $@ $^

example6.o: example6.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example6
