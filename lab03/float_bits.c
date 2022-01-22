// Extract the 3 parts of a float using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"
#define N_BITS 32

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

// given the 3 components of a float
// return 1 if it is inf, 0 otherwise
int is_positive_infinity(float_components_t f) {
    float_components_t c = f;
    if (c.sign == 0 && c.exponent == 255 && c.fraction == 0) {
        return 1;
    //If it isn't +inf
    } else {
        return 0;
    }
}

// given the 3 components of a float
// return 1 if it is -inf, 0 otherwise
int is_negative_infinity(float_components_t f) {
    float_components_t c = f;
    if (c.sign == 1 && c.exponent == 255 && c.fraction == 0) {
        return 1;
    //If it isn't -inf
    } else {
        return 0;
    }
}

// given the 3 components of a float
// return 1 if it is 0 or -0, 0 otherwise
int is_zero(float_components_t f) {
    float_components_t c = f;
    if (c.exponent == 0 && c.fraction == 0) {
        return 1;
    //If it isn't 0 or -0
    } else {
        return 0;
    }
}
