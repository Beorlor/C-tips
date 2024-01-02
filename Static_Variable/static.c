#include <stdio.h>

void countFunction()
{
    // Declare a static variable
    static int count = 0;
    count++;
    printf("This function has been called %d times.\n", count);
}

int main()
{
    countFunction(); // Count is 1
    countFunction(); // Count is 2
    countFunction(); // Count is 3
    return 0;
}
