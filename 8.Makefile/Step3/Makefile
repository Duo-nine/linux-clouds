BIN=test
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(BIN):$(OBJS)
	gcc $^ -o $@

%.o:%.c
	gcc -c $<

clean:
	rm -f $(OBJS)
distclean:
	rm -f $(OBJS)
	rm -f $(BIN)
