// Reads characters from its input and writes the same characters to its output
// any upper case letters are replaced by their as lower case equivalent. 
// Program only stops at the end of input.
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <ctype.h>

int lower_case(int character);

int main(void) {
    //Declare a character and read from terminal
    int character = getchar();
    //While the character is valid 
    while (character != EOF) {
        //Call the swap case function
        int new_character = lower_case(character);
        //Print out the new modified character 
        putchar(new_character);
        //Set the character to the next input 
        character = getchar();
    }
	return 0;
}

//Function for swapping upper to lower and vice versa
int lower_case(int character) {
    //If the letter is uppercase
    if (character >= 'A' && character <= 'Z'){
        int alphabetPosition = character - 'A';
        return 'a' + alphabetPosition;
    //Otherwise it remains unchanged
    } else {
        return character;
    }
}
