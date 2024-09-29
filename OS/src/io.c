#include "./io.h"

// Track the current cursor's row and column
volatile int cursorCol = 0;
volatile int cursorRow = 0;

// C version of assembly I/O port instructions
// Allows for reading and writing with I/O
void outb(uint16 port, uint8 value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8 inb(uint16 port)
{
   uint8 ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// Set the cursor position
int setcursor(int x, int y)
{
    // Check bounds and wrap if necessary
    if (x >= VGA_WIDTH) {
        y += x / VGA_WIDTH;
        x %= VGA_WIDTH;
    }
    if (y >= VGA_HEIGHT) {
        y %= VGA_HEIGHT;
    }

    cursorCol = x;
    cursorRow = y;

    // Update the cursor position in the VGA hardware
    uint16 position = cursorRow * VGA_WIDTH + cursorCol;
    outb(0x3D4, 0x0F);  // Send low byte of cursor position
    outb(0x3D5, (uint8)(position & 0xFF));
    outb(0x3D4, 0x0E);  // Send high byte of cursor position
    outb(0x3D5, (uint8)((position >> 8) & 0xFF));

    return 0;
}

// Print a character to the screen at the current cursor position
char putchar(char character)
{
    uint16 *video = (uint16*) VIDEO_MEMORY;

    if (character == '\n') {
        cursorCol = 0;
        cursorRow++;
    } else {
        video[cursorRow * VGA_WIDTH + cursorCol] = (TEXT_COLOR << 8) | character;
        cursorCol++;
        if (cursorCol >= VGA_WIDTH) {
            cursorCol = 0;
            cursorRow++;
        }
    }

    // Handle screen wraparound
    if (cursorRow >= VGA_HEIGHT) {
        cursorRow = 0;
    }

    // Move cursor to the next position
    setcursor(cursorCol, cursorRow);

    return character;
}

// Print a string to the display
int printf(char string[])
{
    int count = 0;
    for (int i = 0; string[i] != 0; i++) {
        putchar(string[i]);
        count++;
    }
    return count;
}

// Clear the screen by filling it with spaces and reset the cursor
void clearscreen()
{
    uint16 *video = (uint16*) VIDEO_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video[i] = (TEXT_COLOR << 8) | ' ';
    }

    // Reset cursor position to top-left
    cursorCol = 0;
    cursorRow = 0;
    setcursor(0, 0);
}
