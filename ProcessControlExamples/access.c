#include <stdio.h>
#include <unistd.h>

int main()
{
    // Check if "example.txt" can be read by the current process
    if (access("example.txt", R_OK) != -1)
        printf("I have permission\n");
    else
        printf("I don't have permission\n");

    return (0);
}

/*
 * This program demonstrates using access(2) to check file permissions.
 * - access() checks if the current process can read "example.txt".
 * - R_OK is used as the mode argument to check for read permission.
 * - If access() returns -1, the process does not have the specified permission.
 * - Otherwise, the process has the permission to read the file.
 */
