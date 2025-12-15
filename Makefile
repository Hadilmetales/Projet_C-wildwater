CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: c-wire

c-wire: main.o avl.o
	$(CC) $(CFLAGS) -o c-wire main.o avl.o

main.o: main.c header.h
	$(CC) $(CFLAGS) -c main.c

avl.o: avl.c header.h
	$(CC) $(CFLAGS) -c avl.c

clean:
	rm -f *.o c-wire output_c.txt

