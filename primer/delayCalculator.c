#include "delayCalculator.h"
#include <stdio.h>
#include <stdlib.h>

#define VOLTAGE_FILE "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define SLOTS_FILE "/sys/devices/platform/bone_capemgr/slots"
#define DATA_POINTS 10
const double READING[] = {0.0, 500.0, 1000.0, 1500.0, 2000.0, 2500.0, 3000.0, 3500.0, 4000.0, 4100.0};
const double DELAY[] = {0.0, 2.0, 6.0, 12.0, 25.0, 30.0, 50.0, 100.0, 500.0, 1500.0};

void delayCalculator_getReading(int *a2dReading) {
	FILE *voltage_file = fopen(VOLTAGE_FILE, "r");
	if(voltage_file == NULL) {
		printf("Unable to open voltage_file\n");
		exit(-1);
	}

	int reading = fscanf(voltage_file, "%d", a2dReading);
	if(reading < 0) {
		printf("Unable to read from voltage file\n");
		exit(-1);
	}

	fclose(voltage_file);
}

void delayCalculator_enableCape() {
	FILE *slots_file = fopen(SLOTS_FILE, "w");
	if(slots_file == NULL) {
		printf("Unable to open slots file\n");
		exit(-1);
	}

	int enable_slot = fprintf(slots_file, "%s", "BB-ADC");
	printf("enable_slot: %d\n", enable_slot);
	if(enable_slot < 0) {
		printf("ERROR: unable to write to file (%s)\n", SLOTS_FILE);
	}
	fclose(slots_file);
}

int delayCalculator_determineDelay(int reading) {
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
	// printf("reading: %d, Delay: %fms, lowerReading: %d, upperReading: %d\n", reading, delay, lowerReading, upperReading);
	return (int)delay;
}