#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Function to calculate the sum of 'num' integers
int sum(int num, ...)
{
    va_list args;  // To hold the list of arguments
    int total = 0;

    va_start(args, num); // Initialize 'args' to store all values after 'num'
    for (int i = 0; i < num; i++)
    {
        total += va_arg(args, int); // Access the next integer argument
    }
    va_end(args); // Clean up the system stack

    return total;
}

int main()
{
    printf("Sum of 2, 3, and 4 is %d\n", sum(3, 2, 3, 4));
    printf("Sum of 5, 10, 15, and 20 is %d\n", sum(4, 5, 10, 15, 20));

    return EXIT_SUCCESS;
}
