CC	= gcc
CFLAGS	= -O0 -w -ggdb -marm
LDFLAGS = -fno-stack-protector

objects	= boverflow.o

default: boverflow

.PHONY: default clean clobber

boverflow: $(objects)
	$(CC) -o $@ $^

boverflow.o: boverflow.c

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f boverflow
