#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

//How to properly use Bit fields

//first method
#define BITVALUE(X, N) (((X) >> (N)) & 0x1)

//second method (better one)
#define READONLY    0x1
#define HIDDEN    0x2
#define SYSTEM  0x4
#define VOLUME_LABEL    0x8
#define SUBDIR    0x10
#define ARCHIVE    0x20

void    print_bit_info(uint32_t x)
{
    printf("---------------------------\n");
    printf("x = %d\n", x);
    
    //first methode
    printf("first method :\n");
    int numbits = sizeof(x) * 8;
    for (int i = 0; i < numbits; i++)
    {
        printf("%d", BITVALUE(x, numbits - 1 - i));
    }
    printf("\n");

    printf("second method :\n");
    if (x & READONLY) {
        printf("read only\n");
    }
    if (x & HIDDEN) {
        printf("hidden\n");
    }
    if (x & SYSTEM) {
        printf("system\n");
    }
    if (x & VOLUME_LABEL) {
        printf("volume label\n");
    }
    if (x & SUBDIR) {
        printf("subdirectory\n");
    }
    if (x & ARCHIVE) {
        printf("archive\n");
    }
    printf("---------------------------\n\n");
}

int main(void)
{
    uint32_t x = HIDDEN | VOLUME_LABEL | ARCHIVE; // 101010 : 42
    print_bit_info(x);

    //turn bit on (even if already on)
    x = x | SYSTEM; //set

    //turn off a bit (even if already off)
    x = x & ~HIDDEN; //clear

    //inverse a bit
    x = x ^ READONLY; // toggle

    print_bit_info(x);
    return EXIT_SUCCESS;
}
