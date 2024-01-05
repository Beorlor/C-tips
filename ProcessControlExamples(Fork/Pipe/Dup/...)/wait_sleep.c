#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork(); // Fork a new process
    if (pid == -1)
    {
        perror("fork"); // If fork fails, output the error
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) // Child process
    {
        printf("I am the child process.\n");
        sleep(2); // Child process sleeps for 2 seconds
        exit(EXIT_SUCCESS); // Exit child process
    }
    else // Parent process
    {
        printf("I am the parent process.\n");
        wait(NULL); // Parent process waits for the child to terminate
        //int status;
        //waitpid(pid, &status, 0);
        printf("Child process terminated after a 2s delay.\n");
    }

    return (EXIT_SUCCESS);
}

/*
Key Concepts:
- sleep(): Suspends execution of the calling process for a specified number of seconds.
  In this code, the child process sleeps for 2 seconds. This does not consume CPU resources.

- wait(): Makes the parent process wait until all of its child processes terminate.
  If wait(NULL) is used, as in this code, the parent process waits for any child process to end.
  The wait() function is useful for preventing zombie processes and ensuring proper process cleanup.

- The combination of fork(), sleep(), and wait() demonstrates process creation, 
  timed execution, and synchronization between parent and child processes in Unix-like systems.
*/
