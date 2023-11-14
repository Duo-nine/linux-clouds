CC=gcc
CXX=g++
BIN=test
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS=-Wall
LDFLGAS=
INCS=
LIBS=

$(BIN):$(OBJS)
	$(CC) $^ -o $@ $(LDFLGAS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
distclean:
	rm -f $(OBJS)
	rm -f $(BIN)
