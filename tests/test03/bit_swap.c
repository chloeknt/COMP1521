// swap pairs of bits of a 64-bit value, using bitwise operators

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define N_BITS 64

//int swap_bits (uint64_t value, uint64_t first, uint64_t second);

// return value with pairs of bits swapped
uint64_t bit_swap(uint64_t value) {
    //Convert to bits 
    int bits[N_BITS] = {0};
    uint64_t number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = value >> (i - 1);
        if (number & 1) {
            bits[N_BITS - i] = 1;
        } else {
            bits[N_BITS - i] = 0;
        }
    }
    //Loop and swap
    int temp = 0;
    for (int i = 0; i < N_BITS - 1; i += 2) {
        temp = bits[i];
        bits[i] = bits[i + 1];
        bits[i + 1] = temp;
    }
    //Convert back to number
    number = 0;
    uint64_t mask = 1;
    for (int i = N_BITS - 1; i >= 0; i--) {
        if (bits[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    return number;
}

