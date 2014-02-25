AS	= as
LD	= ld
SFLAGS  = 
LDFLAGS =

objects	= hello.o

default: hello

.PHONY: default clean clobber

hello: $(objects)
	$(LD) $(LDFLAGS) -o $@ $^

hello.o: hello.s

%.o: %.s
	$(AS) -c $(SFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f hello
