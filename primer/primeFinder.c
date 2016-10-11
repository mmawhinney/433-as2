#include "primeFinder.h"
#include "delayCalculator.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

static unsigned int primeCounter = 0;
static unsigned long long *primes;
static _Bool isCalculating;
static FILE *writeStream;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sleepForDelay() {
	int delay = delayCalculator_getDelay();
	struct timespec reqDelay;
	reqDelay.tv_sec = (time_t)(delay/1000);
	reqDelay.tv_nsec = (delay % 1000) * 1000000;
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

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
	if(counter % 2 == 0) {
		return;
	}
	for(int i = 3; i <= sqrt(counter); i += 2) {
		if(counter % i == 0) {
			return;		
		}
	}
	primeCounter++;
	addToArray(counter);
	writeToPipe(counter);
	sleepForDelay();
}

void* PrimeFinder_launchThread(void* args) {
	isCalculating = true;
	primes = malloc(sizeof(*primes));

	ThreadArgs *arguments = (ThreadArgs*) args;
	unsigned long long counter = arguments->counter;
	int fileDesc = arguments->fileDesc;

	writeStream = fdopen(fileDesc, "w");
	

	while(isCalculating) {
		findPrimes(counter);
		counter++;
	}

	close(fileDesc);
	fclose(writeStream);
	free(primes);
    return NULL;
}

unsigned int PrimeFinder_getNumPrimesFound() {
	return primeCounter;
}

unsigned long long PrimeFinder_getPrimeByIndex(int index) {
	if(index <= primeCounter && index > 0) {
		return primes[index - 1];
	}
	return 0;
}

_Bool PrimeFinder_isCalculating() {
	return isCalculating;
}

void PrimeFinder_stopCalculating() {
	pthread_mutex_lock(&mutex); 
	{
		isCalculating = false;
	}
	pthread_mutex_unlock(&mutex);
}
