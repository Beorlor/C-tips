#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;

    // Open a file called "example.txt" for writing
    fd = open("example.txt", O_WRONLY | O_CREAT, 0644);

    // Redirect stdout to the file descriptor returned by open
    dup2(fd, STDOUT_FILENO);

    // Close the original file descriptor
    close(fd);

    // Output now goes to "example.txt"
    printf("This is printed in example.txt!\n");

    return (0);
}

/*
 * This program demonstrates using dup2(2) for file descriptor redirection.
 * - open() is called to create or open "example.txt" for writing.
 * - dup2() then duplicates this file descriptor to stdout (file descriptor 1).
 * - As a result, any output normally written to stdout, like with printf(),
 *   is now written to "example.txt".
 */
