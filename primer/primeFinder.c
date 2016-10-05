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

typedef struct ThreadArgs {
	int fileDesc;
	unsigned long long counter;
} ThreadArgs;

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

void writeToPipe(int fileDesc, unsigned long long prime) {
	// FILE *writeStream = fdopen(fileDesc, "w");
	fprintf(writeStream, "%llu\n", prime);
	fflush(writeStream);
	// close(fileDesc);
}

void findPrimes(unsigned long long counter, int fileDesc) {
	for(int i = 2; i <= sqrt(counter); i++) {
		if(counter % i == 0) {
			return;		
		}
	}
	primeCounter++;
	addToArray(counter);
	writeToPipe(fileDesc, counter);
	// printf("Prime found: %llu\n", counter);
}

void* workFunction(void *args) {
	ThreadArgs *arguments = (ThreadArgs*) args;
	unsigned long long counter = arguments->counter;
	int fileDesc = arguments->fileDesc;
	while(counter < 5000005000) {
		findPrimes(counter, fileDesc);
		counter++;
	}
	return NULL;
}

void PrimeFinder_launchThread(int pipeFileDesc) {
	pthread_t tid;
	primes = malloc(sizeof(*primes));
	ThreadArgs args = {pipeFileDesc, 5000000000};
	writeStream = fdopen(pipeFileDesc, "w");
	printf("fds: %d\n", pipeFileDesc);

	pthread_create(&tid, NULL, &workFunction, (void*) &args);

	pthread_join(tid, NULL);
	close(pipeFileDesc);
	for(int i = 0; i < primeCounter; i++) {
		printf("prime: %llu\n", primes[i]);
	}
	free(primes);
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