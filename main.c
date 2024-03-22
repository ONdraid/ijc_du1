#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "error.h"
#include "bitset.h"

#define BITS 128

int main()
{
    bitset_create(sex, BITS);
    bitset_fill(sex, true);
    bitset_setbit(sex, 126, false);

    for (int i = BITS - 1; i >= 0; i--) {
        printf("%ld", bitset_getbit(sex, i));
    }
    putchar('\n');

    return 0;
}
