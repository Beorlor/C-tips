// Inline functions can boost performance by eliminating function call overhead.
// The 'inline' keyword suggests to the compiler that the function's code 
// should be placed directly where it's called, reducing the overhead of a call.

// It's particularly useful for small, frequently used functions. However, 
// it's just a request to the compiler; the compiler may choose to ignore it.

#include <stdio.h>

inline int add(int a, int b)
{
    return a + b; // A simple addition, good candidate for inlining
}

int main(void)
{
    int result = add(5, 3);
    printf("The result is: %d\n", result);
    return 0;
}

// Note:
// - Inline functions should be defined before their first use in the file.
// - They are typically used in header files to allow inlining across multiple source files.
// - Overusing 'inline' can lead to larger binary sizes ('code bloat').
// - Inline functions can make debugging more challenging.
// - The compiler's decision to inline a function may vary based on its optimization settings and the specific function's complexity.
