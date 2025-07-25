/**
 * @file bitset.h
 * @brief Header file for bitset.c.
 * @author Ondřej Vomáčka (xvomaco00), FIT
 * @date 15/03/2024
 *
 * @note First IJC homework part A) solution.
 * @note Compiled with GCC 11.4.0.
 */

#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef IJC_DU1_BITSET_H
#define IJC_DU1_BITSET_H

// Bit array type (for passing a parameter to the function by reference).
typedef unsigned long int *bitset_t;
// Type of the bit array index.
typedef unsigned long int bitset_index_t;

/**
 * @brief Calculates the size of the bitset array.
 *
 * @param bitset_size The size of the bitset (in bits).
 */
#define bitset_size_to_array_length(bitset_size) \
    (1 + (bitset_size / (sizeof(unsigned long) * CHAR_BIT)) + (0 < bitset_size % (sizeof(unsigned long) * CHAR_BIT)))

/**
 * @brief Defines and initializes the given bitset_name variable.
 *
 * @param bitset_name The name of the bitset variable.
 * @param bitset_size The size of the bitset (in bits).
 */
#define bitset_create(bitset_name, bitset_size) \
    static_assert(bitset_size > 0); \
    bitset_index_t bitset_name[bitset_size_to_array_length(bitset_size)] = {bitset_size}

/**
 * @brief Defines and allocates the given bitset_name variable on heap.
 *
 * @param bitset_name The name of the bitset variable.
 * @param bitset_size The size of the bitset (in bits).
 */
#define bitset_alloc(bitset_name, bitset_size) \
    static_assert(bitset_size > 0); \
    bitset_t bitset_name = calloc(bitset_size_to_array_length(bitset_size), sizeof(bitset_index_t)); \
    if (bitset_name == NULL) \
    { \
        error_exit("bitset_alloc: memory allocation failure.\n"); \
    } \
    bitset_name[0] = bitset_size

#ifndef USE_INLINE // If USE_INLINE is not defined, the functions will be defined as macros.

/**
 * @brief Frees the given bitset_name variable from heap.
 *
 * @param bitset_name The name of the bitset variable.
 */
#define bitset_free(bitset_name) free(bitset_name)

/**
 * @brief Retrieves the bitset size (in bits) from the given bitset_name variable.
 *
 * @param bitset_name The name of the bitset variable.
 */
#define bitset_size(bitset_name) bitset_name[0]

/**
 * @brief Fills the given bitset_name variable with zeros or ones.
 *
 * @param bitset_name The name of the bitset variable.
 * @param value If given true, the bitset will be filled with ones, otherwise with zeros.
 */
#define bitset_fill(bitset_name, value) \
    do { \
        for (bitset_index_t i = 1; i < bitset_size_to_array_length(bitset_size(bitset_name)); i++) { \
            if (value) bitset_name[i] = ULONG_MAX; \
            else bitset_name[i] = 0; \
        } \
    } while (0)

/**
 * @brief Sets the bit at the given index in the bitset_name variable to the given value.
 *
 * @param bitset_name The name of the bitset variable.
 * @param value If given true, the bit will be set to one, otherwise to zero.
 */
#define bitset_setbit(bitset_name, index, value) \
    do { \
        if ((long)index > (long)bitset_size(bitset_name)) \
        { \
            error_exit("bitset_setbit: index %lu out of range 0..%lu.\n", \
            (unsigned long)index, (unsigned long)bitset_size(bitset_name)); \
        } \
        bitset_index_t array_index = 1 + index / (sizeof(unsigned long) * CHAR_BIT); \
        unsigned int bit_index = index % (sizeof(unsigned long) * CHAR_BIT); \
        if (value) bitset_name[array_index] |= (unsigned long)1 << bit_index; \
        else bitset_name[array_index] &= ~((unsigned long)1 << bit_index); \
    } while (0)

/**
 * @brief Retrieves the bit at the given index in the bitset_name variable.
 *
 * @param bitset_name The name of the bitset variable.
 * @param index The index of the bit to be retrieved.
 */
#define bitset_getbit(bitset_name, index) \
    (((long)index > (long)bitset_size(bitset_name)) ? \
    error_exit("bitset_getbit: index %lu out of range 0..%lu.\n", \
    (unsigned long)index, (unsigned long)bitset_size(bitset_name)), 69 \
    : \
    (bitset_name[1 + index / (sizeof(unsigned long) * CHAR_BIT)] >> (index % (sizeof(unsigned long) * CHAR_BIT))) & 1)

#else // If USE_INLINE is defined, the functions will be defined as inline functions.

/**
 * @brief Frees the given bitset_name variable from heap.
 *
 * @param bitset_name The name of the bitset variable.
 */
inline void bitset_free(bitset_t bitset_name)
{
    free(bitset_name);
}

/**
 * @brief Retrieves the bitset size (in bits) from the given bitset_name variable.
 *
 * @param bitset_name The name of the bitset variable.
 */
inline bitset_index_t bitset_size(bitset_t bitset_name)
{
    return bitset_name[0];
}

/**
 * @brief Fills the given bitset_name variable with zeros or ones.
 *
 * @param bitset_name The name of the bitset variable.
 * @param value If given true, the bitset will be filled with ones, otherwise with zeros.
 */
inline void bitset_fill(bitset_t bitset_name, bool value)
{
    for (bitset_index_t i = 1; i < bitset_size_to_array_length(bitset_size(bitset_name)); i++) {
        if (value) bitset_name[i] = ULONG_MAX;
        else bitset_name[i] = 0;
    }
}

/**
 * @brief Sets the bit at the given index in the bitset_name variable to the given value.
 *
 * @param bitset_name The name of the bitset variable.
 * @param value If given true, the bit will be set to one, otherwise to zero.
 */
inline void bitset_setbit(bitset_t bitset_name, bitset_index_t index, bool value)
{
        if ((long)index > (long)bitset_size(bitset_name))
        {
            error_exit("bitset_setbit: index %lu out of range 0..%lu.\n",
            (unsigned long)index, (unsigned long)bitset_size(bitset_name));
        }
        bitset_index_t array_index = 1 + index / (sizeof(unsigned long) * CHAR_BIT);
        unsigned int bit_index = index % (sizeof(unsigned long) * CHAR_BIT);
        if (value) bitset_name[array_index] |= (unsigned long)1 << bit_index;
        else bitset_name[array_index] &= ~((unsigned long)1 << bit_index);
}

/**
 * @brief Retrieves the bit at the given index in the bitset_name variable.
 *
 * @param bitset_name The name of the bitset variable.
 * @param index The index of the bit to be retrieved.
 */
inline bool bitset_getbit(bitset_t bitset_name, bitset_index_t index)
{
    if ((long)index > (long)bitset_size(bitset_name))
    {
        error_exit("bitset_getbit: index %lu out of range 0..%lu.\n",
        (unsigned long)index, (unsigned long)bitset_size(bitset_name));
    }
    return
    (bitset_name[1 + index / (sizeof(unsigned long) * CHAR_BIT)] >> (index % (sizeof(unsigned long) * CHAR_BIT))) & 1;
}

#endif //USE_INLINE

#endif //IJC_DU1_BITSET_H
