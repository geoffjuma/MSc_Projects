CC	= gcc 
CFLAGS	= -pthread -I/usr/include -I/usr/include/arm-linux-gnueabi -static

objects = atomic.o mutex.o

default: atomic

.PHONY: default clean clobber

atomic: $(objects)
	$(CC) $(CFLAGS) -o$@ $^

atomic.o: atomic.c
mutex.o: mutex.s

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f atomic
