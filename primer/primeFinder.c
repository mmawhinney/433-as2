#include "primeFinder.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>

unsigned int primeCounter = 0;

void findPrimes(unsigned long long counter) {
	for(int i = 2; i <= sqrt(counter); i++) {
		if(counter % i == 0) {
			return;		
		}
	}
	primeCounter++;
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
	pthread_create(&tid, NULL, &workFunction, NULL);

	pthread_join(tid, NULL);

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