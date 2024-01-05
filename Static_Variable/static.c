#include <stdio.h>

void countFunction()
{
    // Declare a static variable 'count'.
    // A static variable retains its value between function calls.
    // Initially, it's set to 0. Each time this function is called, 
    // 'count' is incremented by 1, and this new value persists 
    // for the next call. This behavior makes 'count' keep track 
    // of the total number of times this function has been called.
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
