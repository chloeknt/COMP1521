#include "float_exp.h"

#define N_BITS 32 

// given the 32 bits of a float return the exponent
uint32_t float_exp(uint32_t f) {
    //Convert the int into an array
    int num[N_BITS] = {0};
    uint32_t number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = f >> (i - 1);
        if (number & 1) {
            num[N_BITS - i] = 1;
        } else {
            num[N_BITS - i] = 0;
        }
    }
    //Extract exponent part 
    number = 0;
    uint32_t mask = 1;
    for (int i = N_BITS - 24; i > 0; i--) {
        if (num[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    return number;
}
