CC = gcc

# compiles to debug
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -pedantic

# compiles to release
OFLAGS = -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic

release: 
	$(CC) $(OFLAGS) *.c *.h -o bfc

test: 
	$(CC) $(CFLAGS) *.c *.h -o bfc

memory: test
	valgrind -s --leak-check=full ./bfc input.bf

clean:
	rm -f vgcore.* *.o bfc
