CC	= gcc
CFLAGS	= -O0 -ggdb -Wall
LDFLAGS = -fno-stack-protector

objects	= example3.o divide.o

default: example3

.PHONY: default clean clobber

example3: $(objects)
	$(CC) -o $@ $^

example3.o: example3.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
%.o: %.s
	as -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f example3
