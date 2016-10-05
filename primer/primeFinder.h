// primeFinder.h
// Module to spawn a sperate thread to find prime numbersand
// store them in a synamically allocated array, push to another
// thread via a pipe, and respond to other commands
#ifndef _PRIME_FINDER_H_
#define _PRIME_FINDER_H_

typedef struct ThreadArgs {
	int fileDesc;
	unsigned long long counter;
} ThreadArgs;

// Begin computing primes on a separate thread. Given the pipe
// handle into which it should push any primes found.
void* PrimeFinder_launchThread(void *args);

// Return how many primes have been found
unsigned int PrimeFinder_getNumPrimesFound(void);

// Get a prime by its index. Returns 0 if index is out of range
unsigned long long PrimeFinder_getPrimeByIndex(int index);

// Return true if currently computing primes
_Bool PrimeFinder_isCalculating(void);

// Stop calculating primes and free all memory
void PrimeFinder_stopCalculating(void);

// Setup delay between primes:
// (the use of this is described in the A2D assignment section)
void PrimeFinder_setDelayBetweenPrimes(int delayInMs);

#endif