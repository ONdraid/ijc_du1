/**
 * @file primes.c
 * @brief Wrapper for the Eratosthenes algorithm.
 * @author Ondřej Vomáčka (xvomaco00)
 * @date 15/03/2024
 *
 * @note First IJC homework part A) solution.
 * @note Compiled with GCC 11.4.0.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

#define N 666000000 // Upper bound for the prime numbers.
#define NUM_TO_PRINT 10 // Number of (last) primes to print.

void print_last_primes(bitset_t bitset, unsigned long num_of_primes, unsigned long n)
{
    while (--n > 1) {
        if (bitset_getbit(bitset, n) == 0) continue;

        if (num_of_primes > 1) print_last_primes(bitset, num_of_primes - 1, n - 1);
        printf("%lu\n", n);
        break;
    }
}

int main(void)
{
    clock_t start = clock();
    bitset_create(primes_bitset, N);
    Eratosthenes(primes_bitset);
    print_last_primes(primes_bitset, NUM_TO_PRINT, N);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}