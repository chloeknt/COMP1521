// generate the opcode for an addi instruction

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "addi.h"

#define N_BITS 32

// return the MIPS opcode for addi $t,$s, i
uint32_t addi(int t, int s, int i) {
    //001000ss sssttttt IIIIIIII IIIIIIII
    int bits[N_BITS] = {0};
    for (int j = 0; j < 6; j++) {
        if (j != 2) {
            bits[j] = 0;
        } else {
            bits[j] = 1;
        }
    }
    int number = 0;
    for (int j = 5; j > 0; j--) {
        number = s >> (j - 1);
        if (number & 1) {
            bits[11 - j] = 1;
        } else {
            bits[11 - j] = 0;
        }
    }
    for (int j = 5; j > 0; j--) {
        number = t >> (j - 1);
        if (number & 1) {
            bits[16 - j] = 1;
        } else {
            bits[16 - j] = 0;
        }
    }
    for (int j = 16; j > 0; j--) {
        number = i >> (j - 1);
        if (number & 1) {
            bits[N_BITS - j] = 1;
        } else {
            bits[N_BITS - j] = 0;
        }
    }
    uint32_t opcode = 0;
    uint32_t mask = 1;
    for (int j = N_BITS - 1; j >= 0; j--) {
        if (bits[j] == 1) {
            opcode = opcode | mask;
        } 
        if (j != 0) {
            mask = mask << 1;
        }
    }
    return opcode;
}
