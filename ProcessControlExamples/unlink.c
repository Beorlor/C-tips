#include <stdio.h>
#include <unistd.h>

int main()
{
    // Attempt to unlink (delete) the file named "example.txt"
    if (unlink("example.txt") == 0)
        printf("File successfully deleted\n"); // If successful, print confirmation
    else
        printf("Error deleting file\n");       // If unsuccessful, print error message

    return (0);
}

/*
Key Concept of unlink():
- unlink() is used to delete a file in Unix-like systems.
- It removes the directory entry for the file, effectively deleting its name.
- The actual data of the file is not immediately removed from disk.
- Deletion of data occurs when the file's link count drops to zero.
  This happens when no process has the file open, and there are no other file names
  (links) associated with the file.
- If the file is open by any process at the time of unlinking, the data remains
  on disk until all references to the file are closed.
- unlink() returns 0 on success and -1 on failure, setting errno to indicate the error.
*/
