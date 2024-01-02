#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void simple_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format != '\0')
    {
        if (*format == 'd')
        {
            int i = va_arg(args, int);
            printf("%d\n", i);
        } 
        else if (*format == 'c')
        {
            // Note: char is promoted to int when passed through '...'
            int c = va_arg(args, int);
            printf("%c\n", c);
        } 
        else if (*format == 'f')
        {
            double d = va_arg(args, double);
            printf("%f\n", d);
        }
        else if (*format == 's')
        {
            char *s = va_arg(args, char *);
            printf("%s\n", s);
        }

        ++format;
    }
    va_end(args);
}

int main()
{
    simple_printf("dcfs", 3, 'a', 3.14, "hello world");
    return EXIT_SUCCESS;
}
