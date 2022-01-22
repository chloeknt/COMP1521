// Swap bytes of a short

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define N_BITS 16

// given uint16_t value return the value with its bytes swapped
uint16_t short_swap(uint16_t value) {
    //Convert to bits 
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
    //Separate into two groups of eight
    int first[N_BITS / 2] = {0};
    int j = 0;
    for (int i = 0; i < N_BITS / 2; i++) {
        first[j] = bits[i];
        j++;
    }
    j = 0;
    int second[N_BITS/2] = {0};
    for (int i = 8; i < N_BITS; i++) {
        second[j] = bits[i];
        j++;
    }
    //Swap positions and convert to int16_t
    int swapped[N_BITS] = {0};
    j = 0;
    for (int i = 0; i < N_BITS / 2; i++) {
        swapped[j] = second[i];
        j++;
    }
    for (int i = 0; i < N_BITS / 2; i++) {
        swapped[j] = first[i];
        j++;
    }
    number = 0;
    int mask = 1;
    for (int i = N_BITS - 1; i >= 0; i--) {
        if (swapped[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    return number;
}
