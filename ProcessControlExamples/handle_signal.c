#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/*
 * This program demonstrates custom signal handling in C.
 * - SIGTSTP (usually triggered by Ctrl+Z) can have its default behavior (which is to pause a program)
 *   overridden by a custom handler. In this program, we set up such a handler that prevents the program
 *   from being paused, instead printing a message that stopping is not allowed.
 * - SIGCONT, on the other hand, cannot have its default behavior of resuming a program overridden; 
 *   additional behaviors can be added, but the continuation of the program cannot be stopped.
 * - The program prompts the user to enter a number, reads it, multiplies it by 5, and prints the result.
 * - The third argument of `sigaction`, when not NULL, points to a `struct sigaction` where the previous
 *   action for the signal is saved. This can be used to restore the old signal action later if needed.
 *   Here, it's set to NULL as we don't intend to restore the old action.
 */

// Handler function for the SIGTSTP signal.
void handle_sigtstp(int sig) {
    // Print a message instead of allowing the program to be stopped.
    printf("Stop not allowed\n");
}

int main(int argc, char* argv[]) {
    // Set up the sigaction structure to specify the new action for SIGTSTP.
    struct sigaction sa, old_sa;
    
    // Assign the handler function for SIGTSTP.
    sa.sa_handler = &handle_sigtstp;
    
    // Set SA_RESTART so system calls interrupted by this signal are restarted.
    sa.sa_flags = SA_RESTART;
    
    // Change the action for SIGTSTP and save the old action in old_sa.
    if (sigaction(SIGTSTP, &sa, &old_sa) == -1) {
        perror("Could not set custom SIGTSTP handler");
        return EXIT_FAILURE;
    };

    int x; // Variable to store user input.

    printf("Input number: "); // Prompt for input.
    scanf("%d", &x); // Read an integer from the input.
    printf("Result %d * 5 = %d\n", x, x * 5); // Print the result of x multiplied by 5.
    
    // Restore the original action for SIGTSTP before exiting.
    if (sigaction(SIGTSTP, &old_sa, NULL) == -1) {
        perror("Could not restore original SIGTSTP handler");
        return EXIT_FAILURE;
    }

    return 0; // Exit the program normally.
}
