CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra -Werror -fsanitize=address
LDFLAGS=-fsanitize=address

all: no-comment pre-test test

pre-test: main.c error.o bitset.h
	$(CC) $(CFLAGS) -o main.i -E main.c

test: main.c error.o bitset.h
	$(CC) $(CFLAGS) -o test main.c error.o $(LDFLAGS)
	./test

no-comment: no-comment.o error.o
	$(CC) $(CFLAGS) -o no-comment no-comment.o error.o $(LDFLAGS)

no-comment.o: no-comment.c
	$(CC) $(CFLAGS) -c no-comment.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

clean:
	rm -f *~ *.bak *.o *.zip

archive:
	zip xvomaco00.zip *.c *.h Makefile
