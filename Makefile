CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra -Werror -fsanitize=address
LDFLAGS=-fsanitize=address

all: primes no-comment

run: primes
	./primes

primes: primes.o error.o eratosthenes.o bitset.h
	$(CC) $(CFLAGS) -o primes primes.o error.o eratosthenes.o $(LDFLAGS) -lm

primes.o: primes.c bitset.h
	$(CC) $(CFLAGS) -c primes.c

no-comment: no-comment.o error.o
	$(CC) $(CFLAGS) -o no-comment no-comment.o error.o $(LDFLAGS)

no-comment.o: no-comment.c
	$(CC) $(CFLAGS) -c no-comment.c

eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -c eratosthenes.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

clean:
	rm -f *~ *.bak *.o *.zip

archive:
	zip xvomaco00.zip *.c *.h Makefile
