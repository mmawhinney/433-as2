#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define VOLTAGE_FILE "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

static pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

void enableI2cBus() {
	pthread_mutex_lock(&fileMutex); 
	{
		FILE *slots = fopen(SLOTS, "w");
		if(slots == NULL) {
			printf("Unable to open file (%s) for writing\n", SLOTS);
			exit(-1);
		}

		int value = fprintf(slots, "%s", "BB-I2C1");
		if(value < 0) {
			printf("Unable to write to file %s\n", SLOTS);
			exit(-1);
		}
		fclose(slots);
	}
	pthread_mutex_unlock(&fileMutex);
}

void delayCalculator_enableCape() {
	pthread_mutex_lock(&fileMutex);	
	{
		FILE *slotsFile = fopen(SLOTS, "w");
		if(slotsFile == NULL) {
			printf("Unable to open slots file\n");
			exit(-1);
		}

		int enableSlot = fprintf(slotsFile, "%s", "BB-ADC");
		if(enableSlot < 0) {
			printf("ERROR: unable to write to file (%s)\n", SLOTS);
			exit(-1);
		}
		fclose(slotsFile);
		
	}
	pthread_mutex_unlock(&fileMutex);
}

void delayCalculator_getReading(int *a2dReading) {
	pthread_mutex_lock(&fileMutex);
	{
		int counter = 0;
		FILE *voltage_file;
		do {
			voltage_file = fopen(VOLTAGE_FILE, "r");
			counter++;
			struct timespec sleep = {0, 5*1000000};
			nanosleep(&sleep, (struct timespec *) NULL);
		} while(voltage_file == NULL && counter < 50);
		
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
	pthread_mutex_unlock(&fileMutex);
}