#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int bcd(int bcd_value);

#define N_BITS 16

int main(int argc, char *argv[]) {

    for (int arg = 1; arg < argc; arg++) {
        long l = strtol(argv[arg], NULL, 0);
        assert(l >= 0 && l <= 0x0909);
        int bcd_value = l;

        printf("%d\n", bcd(bcd_value));
    }

    return 0;
}

// given a  BCD encoded value between 0 .. 99
// return corresponding integer
int bcd(int bcd_value) {
    //Determining the binary format of the BCD value 
    char *string = malloc(N_BITS + 1);
    int number = 0;
    for (int i = N_BITS; i > 0; i--) {
        number = bcd_value >> (i - 1);
        if (number & 1) {
            string[N_BITS - i] = '1';
        } else {
            string[N_BITS - i] = '0';
        }
    }
    string[N_BITS] = '\0'; 
    //Converting to integer  
    int result = 0;
    int tens = 10;
    for (int byte = 0; byte <= (N_BITS) / 2; byte += 8) {
        int place_result = 0;
        int mask = 1;
        for(int i = 7; i >= 0; i--) {
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
    
