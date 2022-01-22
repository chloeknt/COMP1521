#include "bit_rotate.h"

#define N_BITS 16 

void rotate_left(int num[], int n_rotations);
void rotate_right(int num[], int n_rotations);

// return the value bits rotated left n_rotations
uint16_t bit_rotate(int n_rotations, uint16_t bits) {
    //Convert the int into an array
    int num[N_BITS] = {0};
    uint16_t number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = bits >> (i - 1);
        if (number & 1) {
            num[N_BITS - i] = 1;
        } else {
            num[N_BITS - i] = 0;
        }
    }
    //Rotate
    if (n_rotations < 0) {
        n_rotations *= -1;
        for (int i = 0; i < n_rotations; i++) {
            rotate_right(num, n_rotations);
        }
    } else {
        for (int i = 0; i < n_rotations; i++) {
            rotate_left(num, n_rotations);
        }
    }
    //Convert back 
    number = 0;
    uint16_t mask = 1;
    for (int i = N_BITS - 1; i >= 0; i--) {
        if (num[i] == 1) {
            number = number | mask;
        } 
        if (i != 0) {
            mask = mask << 1;
        }
    }
    return number;
}

void rotate_left(int num[], int n_rotations) {
    int temp = num[0];
    for (int i = 0; i < N_BITS - 1; i++) {
        num[i] = num[i + 1];
    }
    num[N_BITS - 1] = temp;
}

void rotate_right(int num[], int n_rotations) {
    int temp = num[N_BITS - 1];
    for (int i = N_BITS - 1; i > 0; i--) {
        num[i] = num[i - 1];
    }
    num[0] = temp;
}
