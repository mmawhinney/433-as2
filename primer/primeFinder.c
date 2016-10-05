#include "primeFinder.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned int primeCounter = 0;
unsigned long long *primes;

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

void findPrimes(unsigned long long counter) {
	for(int i = 2; i <= sqrt(counter); i++) {
		if(counter % i == 0) {
			return;		
		}
	}
	primeCounter++;
	addToArray(counter);
	printf("Prime found: %llu\n", counter);
}

void* workFunction(void *args) {
	unsigned long long counter = 5000000000;
	while(counter < 5000005000) {
		findPrimes(counter);
		counter++;
	}
	return NULL;
}

void PrimeFinder_launchThread(/*int pipeFileDesc*/) {
	pthread_t tid;
	primes = malloc(sizeof(*primes) * 1);
	pthread_create(&tid, NULL, &workFunction, NULL);

	pthread_join(tid, NULL);

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