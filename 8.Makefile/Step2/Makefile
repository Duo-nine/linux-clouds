BIN=test

$(BIN):main.o a.o b.o c.o
	gcc $^ -o $@

main.o:main.c
	gcc -c $< -o $@
a.o:a.c
	gcc -c $< -o $@
b.o:b.c
	gcc -c $< -o $@
c.o:c.c
	gcc -c $< -o $@

clean:
	rm -f main.o a.o b.o c.o
distclean:
	rm -f main.o a.o b.o c.o
	rm -f $(BIN)
