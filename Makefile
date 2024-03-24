# @author Ondřej Vomáčka (xvomaco00)
# @date 15/03/2024

CC=gcc
CFLAGS=-O2 -std=c11 -pedantic -Wall -Wextra -Werror# Optimized
LDFLAGS=-Wall -Wextra -Werror
# CFLAGS=-g -std=c11 -pedantic -Wall -Wextra -Werror -fsanitize=address# Debug
# LDFLAGS=-Wall -Wextra -Werror -fsanitize=address

ifeq ($(32bit), 1)
CFLAGS += -m32
LDFLAGS += -m32
endif

all: primes primes-i no-comment

run: primes primes-i
	ulimit -s unlimited && ./primes
	ulimit -s unlimited && ./primes-i

primes: primes.o eratosthenes.o bitset.h error.o
	$(CC) $(CFLAGS) -o primes primes.o error.o eratosthenes.o $(LDFLAGS) -lm

pre-primes: primes.c eratosthenes.c bitset.h error.c
	$(CC) $(CFLAGS) -E primes.c error.c eratosthenes.c $(LDFLAGS) -lm > primes.i

primes-i: primes-i.o eratosthenes-i.o bitset.h error.o
	$(CC) $(CFLAGS) -o primes-i -DUSE_INLINE primes-i.o eratosthenes-i.o error.o $(LDFLAGS) -lm

pre-primes-i: primes.c eratosthenes.c bitset.h error.c
	$(CC) $(CFLAGS) -E -DUSE_INLINE primes.c eratosthenes.c error.c $(LDFLAGS) -lm > primes-i.i

primes.o: primes.c bitset.h
	$(CC) $(CFLAGS) -c primes.c

primes-i.o: primes.c bitset.h
	$(CC) $(CFLAGS) -o primes-i.o -DUSE_INLINE -c primes.c

eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -c eratosthenes.c

eratosthenes-i.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -o eratosthenes-i.o -DUSE_INLINE -c eratosthenes.c

no-comment: no-comment.o error.o
	$(CC) $(CFLAGS) -o no-comment no-comment.o error.o $(LDFLAGS)

no-comment.o: no-comment.c
	$(CC) $(CFLAGS) -c no-comment.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

archive: clean
	zip xvomaco00.zip *.c *.h Makefile

clean:
	rm -f *~ *.bak *.o *.i *.zip no-comment primes primes-i
