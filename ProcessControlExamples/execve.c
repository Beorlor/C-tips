#include <unistd.h>
#include <stdio.h>

int main()
{
    char *args[3];

    args[0] = "ls";    // Command to execute
    args[1] = "-l";    // Argument: detailed listing
    args[2] = NULL;    // Array must be NULL-terminated

    // Call execve to execute /bin/ls with the specified arguments
    execve("/bin/ls", args, NULL);

    // The following line will not be executed if execve is successful
    printf("This line will not be executed.\n");

    return (0);
}

/*
 * execve() is a system call used to execute a new program, replacing the current process image.
 * - The first argument is the path to the executable ("/bin/ls" in this case).
 * - The second argument is an array of strings (args) that represent the command line arguments.
 * - The third argument is for environment variables, set to NULL in this example.
 * - Upon successful execution of execve, the current process is replaced by the new program,
 *   so any code following execve will not be executed unless execve fails.
 * - The args array includes the command and its arguments, followed by a NULL pointer as the last element.
 * - So the goal is to use execve in a child process.
 */
