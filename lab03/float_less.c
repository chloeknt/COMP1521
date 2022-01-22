// Compare 2 floats using bit operations only

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"
#define N_BITS 32

//-inf -42
//-0.748 -0.749

// float_less is given the bits of 2 floats bits1, bits2 as a uint32_t
// and returns 1 if bits1 < bits2, 0 otherwise
// 0 is return if bits1 or bits2 is Nan
// only bit operations and integer comparisons are used
uint32_t float_less(uint32_t bits1, uint32_t bits2) {
    float_components_t c1 = float_bits(bits1);
    int nan1 = is_nan(c1);
    float_components_t c2 = float_bits(bits2);
    int nan2 = is_nan(c2);
    //If either is NaN
    if (nan1 == 1 || nan2 == 1) {
        return 0;
    //Consider cases where bits1 < bits2
    //Positive exceeds negative
    //sign = 1 is negative, sign = 0 is positive
    } else if (c1.sign > c2.sign) {
        return 1;
    } else if (c1.sign < c2.sign) {
        return 0;
    //Higher exponent exceeds lower exponent in positive
    } else if (c1.sign == 1 && c1.exponent < c2.exponent) {
        return 0;
    } else if (c1.sign == 1 && c1.exponent > c2.exponent) {
        return 1;
    } else if (c1.sign == 0 && c1.exponent < c2.exponent) {
        return 1;
    } else if (c1.sign == 0 && c1.exponent > c2.exponent) {
        return 0;
    //Higher fraction exceeds lower fraction in positive
    } else if (c1.sign == 1 && c1.fraction < c2.fraction) {
        return 0;
    } else if (c1.sign == 1 && c1.fraction > c2.fraction) {
        return 1;
    } else if (c1.sign == 0 && c1.fraction < c2.fraction) {
        return 1;
    } else {
    //bits1 >= bits2 OR Nan
        return 0;
    }
}

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {
    //Convert the int into an array
    int num[N_BITS] = {0};
    int number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = f >> (i - 1);
        if (number & 1) {
            num[N_BITS - i] = 1;
        } else {
            num[N_BITS - i] = 0;
        }
    }
    //Initialise a struct to store values 
    float_components_t *components = malloc(sizeof(float_components_t));
    //Sign
    //If [0] is 1 = neg 0 = pos
    components->sign = num[0];
    //Exponent
    //[1] to [8], convert to int from binary
    number = 0;
    int mask = 1;
    for (int i = N_BITS - 24; i > 0; i--) {
        if (num[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    components->exponent = number;
    //Fraction
    //[9] to [31]
    number = 0;
    mask = 1;
    for (int i = N_BITS - 1; i > 8; i--) {
        if (num[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    components->fraction = number;
    return *components;
}

// given the 3 components of a float
// return 1 if it is NaN, 0 otherwise
int is_nan(float_components_t f) {
    float_components_t c = f;
    if (c.sign == 0 && c.exponent == 255 && c.fraction == 4194304) {
        return 1;
    //If it isn't NaN
    } else {
        return 0;
    }
}
