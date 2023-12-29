// Its very crucial to use volatile when reading micro controllers inputs

// The volatile keyword in C and C++ programming languages is used to tell the
// compiler that a variable's value may change at any time without any action
// being taken by the code the compiler finds nearby. Its primary purpose is
// to prevent the compiler from applying any optimizations on the variable that
// could change the timing or the order of reads and writes to that variable.

// clang -O2 Volatile.c
// if we use the optimise compiler, it will change while(!done) to while(true),
// since there is no reason that done get updated in his scope
// so we use volatile to say "dont optimise this variable"

// Syntax :

// volatile int done; INT is volatile
// volatile *int done; INT is volatile, not the adresse
// int *volatile done; The pointer is volatile
// volatile int *volatine done; Both are volatile

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

// bool done = false;
volatile bool done = false;

void *tfunc(void *arg)
{
    sleep(1);
    done = true;
    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_create(&t1, NULL, tfunc, NULL);

    printf("Waiting...\n");
    while (!done) {}
    printf("Ok. Moving on.\n");
}