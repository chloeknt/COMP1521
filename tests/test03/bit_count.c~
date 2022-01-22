// count bits in a uint64_t

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define N_BITS 64

// return how many 1 bits value contains
int bit_count(uint64_t value) {
    int bits[N_BITS] = {0};
    int number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = value >> (i - 1);
        if (number & 1) {
            bits[N_BITS - i] = 1;
        } else {
            bits[N_BITS - i] = 0;
        }
    }
    int count = 0;
    for (int i = 0; i < N_BITS; i++) {
        if (bits[i] == 1) {
            count++;
        }
    }

    return count;
}
