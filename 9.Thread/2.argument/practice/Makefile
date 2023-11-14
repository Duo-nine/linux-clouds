all:test_A test_B

CFLAGS=-Wall

test_A:a.c
	gcc $(CFLAGS) a.c -o test_A -lpthread
test_B:b.c
	gcc $(CFLAGS) b.c -o test_B -lpthread
