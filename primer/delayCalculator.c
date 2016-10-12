#include "delayCalculator.h"
#include "primeFinder.h"
#include "fileAccessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SLOTS_FILE "/sys/devices/platform/bone_capemgr/slots"
#define DATA_POINTS 10

const double READING[] = {0.0, 500.0, 1000.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4100.0};
const double DELAY[] = {0.0, 2.0, 6.0, 12.0, 25.0, 30.0, 50.0, 100.0, 500.0, 1500.0};
static int primeFindingDelay;
static int primeCount;

void delayCalculator_determineDelay(int reading) {
	int lowerReading = 0;
	int upperReading = 1;
	for(int i = 0; i < DATA_POINTS; i++) {
		if(reading > READING[i]) {
			lowerReading = i;
		} else if(reading < READING[i]) {
			upperReading = i;
			break;
		}
	}

	int s = reading;
	double a = READING[lowerReading];
	double b = READING[upperReading];
	double n = DELAY[upperReading];
	double m = DELAY[lowerReading];
	double delay = (((s-a)/(b-a)) * (n-m)) + m;
	primeFindingDelay = (int)delay;
}

int delayCalculator_getDelay() {
	return primeFindingDelay;
}

int delayCalculator_getNumPrimesInLastSecond() {
	return primeCount;
}

void* delayCalculator_launchThread(void* args) {
	fileAccessor_enableCape();
	while(1) {
		int a2dReading = 0;
		fileAccessor_getReading(&a2dReading);
		delayCalculator_determineDelay(a2dReading);
		printf("Delay is: %dms\n", primeFindingDelay);
		int primeCountBefore = PrimeFinder_getNumPrimesFound();
		struct timespec sleep = {1, 0};
		nanosleep(&sleep, (struct timespec *) NULL);
		int primeCountAfter = PrimeFinder_getNumPrimesFound();
		primeCount = primeCountAfter - primeCountBefore;
		if(!PrimeFinder_isCalculating()) {
			break;
		}
	}
	return NULL;
}