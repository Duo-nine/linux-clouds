CROSS_COMPILE=arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc
CXX=g++
BIN=test
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS=-Wall
LDFLGAS=
INCS=
LIBS=
NFSDIR=/home/meson/nfsroot

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LDFLGAS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
distclean:
	rm -f $(OBJS)
	rm -f $(BIN)
copy:
	cp $(BIN) $(NFSDIR)
