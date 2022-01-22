// Program that reads characters and checks if they are vowels. If they are, 
// the vowel is not printed to the standard output. Program only stops 
// at the end of input.
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <string.h>


//Function for detemining if a character is a lower case vowel
int is_vowel(int character);

int main(void) {
    char letter;
    while (scanf("%c", &letter) != EOF) {
        int result = is_vowel(letter);
        if (result == 0) {
            printf("%c", letter);
        } 
    }
	return 0;
}

int is_vowel(int character) {
    //If the character is a lower case vowel return 1
    if (character == 'a' || character == 'e' 
    || character == 'i'|| character == 'o' || 
    character == 'u'|| character == 'A' || character == 'E' 
    || character == 'I'|| character == 'O' || 
    character == 'U' ) {
        return 1;
        //If not return 0
    } else {
        return 0;
    }
}
