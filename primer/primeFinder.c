#include "primeFinder.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

unsigned int primeCounter = 0;
unsigned long long *primes;
FILE *writeStream;

void addToArray(unsigned long long prime) {
	unsigned long long tempArray[primeCounter-1];
	for(int i = 0; i < primeCounter-1; i++) {
		tempArray[i] = primes[i];
	}
	free(primes);
	primes = malloc(sizeof(*primes) * primeCounter);
	for(int i = 0; i < primeCounter-1; i++) {
		primes[i] = tempArray[i];
	}
	primes[primeCounter-1] = prime;
}

void writeToPipe(unsigned long long prime) {
	fprintf(writeStream, "%llu\n", prime);
	fflush(writeStream);
}

void findPrimes(unsigned long long counter) {
	for(int i = 2; i <= sqrt(counter); i++) {
		if(counter % i == 0) {
			return;		
		}
	}
	primeCounter++;
	addToArray(counter);
	writeToPipe(counter);
}

// void* workFunction(void *args) {
//     ThreadArgs *arguments = (ThreadArgs*) args;
//     unsigned long long counter = arguments->counter;
//     int fileDesc = arguments->fileDesc;
//     while(counter < 5000005000) {
//         findPrimes(counter, fileDesc);
//         counter++;
//     }
//     return NULL;
// }

void* PrimeFinder_launchThread(void* args) {
	
	primes = malloc(sizeof(*primes));

	ThreadArgs *arguments = (ThreadArgs*) args;
	unsigned long long counter = arguments->counter;
	int fileDesc = arguments->fileDesc;

	writeStream = fdopen(fileDesc, "w");
	
    while(counter < 5000005000) {
		findPrimes(counter);
		counter++;
	}

	close(fileDesc);
	free(primes);
    return NULL;
}

unsigned int PrimeFinder_getNumPrimesFound() {
	return primeCounter;
}

unsigned long long PrimeFinder_getPrimeByIndex(int index) {
	return 0;
}

_Bool PrimeFinder_isCalculating() {
	return false;
}

void PrimeFinder_stopCalculating() {

}

void PrimeFinder_setDelayBetweenPrimes(int delayInMs) {

}