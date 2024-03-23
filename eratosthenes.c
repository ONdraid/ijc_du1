/**
 * @file eratosthenes.c
 * @brief Sieve of Eratosthenes algorithm implementation.
 * @author Ondřej Vomáčka (xvomaco00)
 * @date 15/03/2024
 *
 * @note First IJC homework part A) solution.
 * @note Compiled with GCC 11.4.0.
 */

#include <math.h>
#include "error.h"
#include "eratosthenes.h"

void Eratosthenes(bitset_t bitset)
{
    unsigned long bitset_size = bitset_size(bitset);
    bitset_fill(bitset, 1);
    bitset_setbit(bitset, 0, 0); bitset_setbit(bitset, 1, 0);

    for (unsigned long i = 2; i < sqrt(bitset_size); i++) {
        if (!bitset_getbit(bitset, i)) continue;

        for (unsigned long j = i * i; j < bitset_size; j += i) {
            bitset_setbit(bitset, j, 0);
        }
    }
}
