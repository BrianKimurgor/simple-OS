#include "./io.h"
#include "hal.h"  // For scanf and getchar

int main() {
    // Clear the screen at the start
    clearscreen();

    // Initialize the keymap from io.c
    initkeymap();

    // Declare a buffer to hold the user input (up to 100 characters)
    char input[101];

    // Print "Hello World!"
	printf("Hello World!\n");

    

    // Run an infinite loop to allow continuous input and output
    while (1) {
        // Prompt the user to enter a string
        printf("Enter a string: ");
        
        // Capture the user's input using scanf (up to 100 characters)
        scanf(input);
        
        // Print the entered string back to the user
        printf("\nYou entered: ");
        printf(input);
        printf("\n");
    }

    return 0;
}

