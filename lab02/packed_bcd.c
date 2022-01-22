#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define N_BCD_DIGITS 8
#define N_BITS 32

uint32_t packed_bcd(uint32_t packed_bcd);

int main(int argc, char *argv[]) {

    for (int arg = 1; arg < argc; arg++) {
        long l = strtol(argv[arg], NULL, 0);
        assert(l >= 0 && l <= UINT32_MAX);
        uint32_t packed_bcd_value = l;

        printf("%lu\n", (unsigned long)packed_bcd(packed_bcd_value));
    }

    return 0;
}

// given a packed BCD encoded value between 0 .. 99999999
// return the corresponding integer
uint32_t packed_bcd(uint32_t packed_bcd_value) {
    //Determining the binary format of the BCD value 
    char *string = malloc(N_BITS + 1);
    int number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = packed_bcd_value >> (i - 1);
        if (number & 1) {
            string[N_BITS - i] = '1';
        } else {
            string[N_BITS - i] = '0';
        }
    }
    string[N_BITS] = '\0';
    //Interpreting the values separately (tens decimal)
    int result = 0;
    int tens = 10000000;
    for (int byte = 0; byte <= (N_BCD_DIGITS/2) * 7; byte += 4) {
        int place_result = 0;
        int mask = 1;
        for(int i = 3; i >= 0; i--) {
            if (string[byte + i] == '1') {
                place_result = place_result | mask;
            } 
            if (i != 0) {
                mask = mask << 1;
            }
        }  
        place_result *= tens;
        tens /= 10;
        result += place_result;
    }
    return result;
}

