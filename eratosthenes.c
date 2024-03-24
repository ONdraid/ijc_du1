/**
 * @file eratosthenes.c
 * @brief Sieve of Eratosthenes algorithm implementation.
 * @author Ondřej Vomáčka (xvomaco00), FIT
 * @date 15/03/2024
 *
 * @note First IJC homework part A) solution.
 * @note Compiled with GCC 11.4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

#ifdef USE_INLINE

extern bitset_index_t bitset_size(bitset_t bitset_name);
extern void bitset_fill(bitset_t bitset_name, bool value);
extern void bitset_setbit(bitset_t bitset_name, bitset_index_t index, bool value);
extern bool bitset_getbit(bitset_t bitset_name, bitset_index_t index);

#endif

/**
 * @brief Computes prime numbers up to the given number (stored in bitset).
 * @param bitset Prime numbers will be stored in this bitset.
 */
void Eratosthenes(bitset_t bitset)
{
    unsigned long size = bitset_size(bitset);
    bitset_fill(bitset, true);
    bitset_setbit(bitset, 0, false); bitset_setbit(bitset, 1, false);

    for (unsigned long i = 2; i < sqrt(size); i++) {
        if (!bitset_getbit(bitset, i)) continue;

        for (unsigned long j = i * i; j < size; j += i) {
            bitset_setbit(bitset, j, false);
        }
    }
}
