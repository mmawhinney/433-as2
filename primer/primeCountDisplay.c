#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "primeFinder.h"
#include "delayCalculator.h"
#include "primeCountDisplay.h"

#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15
#define I2C_BUS_1 "/dev/i2c-1"
#define SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_44_VALUE "/sys/class/gpio/gpio44/value"
#define GPIO_44_DIRECTION "/sys/class/gpio/gpio44/direction"
#define GPIO_61_VALUE "/sys/class/gpio/gpio61/value"
#define GPIO_61_DIRECTION "/sys/class/gpio/gpio61/direction"
#define GPIO_44_PIN 44
#define GPIO_61_PIN 61

const int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const int hex14[] = {0xA1, 0x80, 0x31, 0xB0, 0x90, 0xB0, 0xB1, 0x04, 0xB1, 0x90};
const int hex15[] = {0x86, 0x12, 0x0E, 0x06, 0x8A, 0x8C, 0x8C, 0x14, 0x8E, 0x8E};

static int primeCount;
static int segments14Left;
static int segments15Left;
static int segments14Right;
static int segments15Right;

static int initI2cBus(char *bus, int address) {
	int i2cFileDesc = open(bus, O_RDWR);
	if(i2cFileDesc < 0) {
		printf("i2c: Unable to open bus for read/write (%s)\n", bus);
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if(result < 0) {
		printf("i2c: Unable to set i2c device to slave address\n");
		exit(-1);
	}
	return i2cFileDesc;
}

static void writeI2cRegister(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if(res != 2) {
		printf("i2c: Unable to write i2c register\n");
		exit(-1);
	}
}

void exportPins() {
	FILE *exportFile = fopen(GPIO_EXPORT, "w");
	if(exportFile == NULL) {
		printf("Unable to open file (%s) for writing\n", GPIO_EXPORT);
		exit(-1);
	}

	int exportPin44 = fprintf(exportFile, "44");
	if(exportPin44 < 0) {
		printf("Unable to write to file (%s)\n", GPIO_EXPORT);
		exit(-1);
	}
	int exportPin61 = fprintf(exportFile, "61");
	if(exportPin61 < 0) {
		printf("Unable to write to file (%s)\n", GPIO_EXPORT);
		exit(-1);
	}
	fclose(exportFile);
}

void setPinsDirection() {
	FILE *direction44File = fopen(GPIO_44_DIRECTION, "w");
	if(direction44File == NULL) {
		printf("Unable to open file (%s) for writing\n", GPIO_44_DIRECTION);
		exit(-1);
	}

	int directionPin44 = fprintf(direction44File, "out");
	if(directionPin44 < 0) {
		printf("Unable to write to file (%s)\n", GPIO_44_DIRECTION);
		exit(-1);
	}
	fclose(direction44File);

	FILE *direction61File = fopen(GPIO_61_DIRECTION, "w");
	if(direction61File == NULL) {
		printf("Unable to open file (%s) for writing\n", GPIO_61_DIRECTION);
		exit(-1);
	}

	int directionPin61 = fprintf(direction61File, "61");
	if(directionPin61 < 0) {
		printf("Unable to write to file (%s)\n", GPIO_61_DIRECTION);
		exit(-1);
	}
	fclose(direction61File);
}

void enableI2cBus() {
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

void driveToPin(int pin, int value) {
	if(pin == 44) {
		FILE *valueFile = fopen(GPIO_44_VALUE, "w");
		if(valueFile == NULL) {
			printf("Unable to open file (%s) for writing\n", GPIO_44_VALUE);
			exit(-1);
		}

		int gpioValue = fprintf(valueFile, "%d", value);
		if(gpioValue < 0) {
			printf("Unable to write to file (%s)\n", GPIO_44_VALUE);
			exit(-1);
		}
		fclose(valueFile);
	} else if(pin == 61) {
		FILE *valueFile = fopen(GPIO_61_VALUE, "w");
		if(valueFile == NULL) {
			printf("Unable to open file (%s) for writing\n", GPIO_61_VALUE);
			exit(-1);
		}

		int gpioValue = fprintf(valueFile, "%d", value);
		if(gpioValue < 0) {
			printf("Unable to write to file (%s)\n", GPIO_61_VALUE);
			exit(-1);
		}
		fclose(valueFile);
	} else {
		printf("Invalid pin specified\n");
		exit(-1);
	}
}

void init() {
	exportPins();
	setPinsDirection();
	enableI2cBus();
}

void displayCount() {
	char num[20];
	snprintf(num, 3, "%d", primeCount);
	int rightVal = num[1] - '0';
	int leftVal = num[0] - '0';
	if(rightVal != -48) {
		for(int i = 0; i < 10; i++) {
			if(rightVal == nums[i]) {
				segments14Right = hex14[i];
				segments15Right = hex15[i];
			}
		}
		for(int i = 0; i < 10; i++) {
			if(leftVal == nums[i]) {
				segments14Left = hex14[i];
				segments15Left = hex15[i];
			}
		}	
	} else {
		for(int i = 0; i < 10; i++) {
			if(leftVal == nums[i]) {
				segments14Right = hex14[i];
				segments15Right = hex15[i];
			}
		}
		segments14Left = hex14[0];
		segments15Left = hex15[0];
	}
	
	
}

void* primeCountDisplay_launchThread(void* args) {
	init();
	int i2cFileDesc = initI2cBus(I2C_BUS_1, I2C_DEVICE_ADDRESS);
	writeI2cRegister(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cRegister(i2cFileDesc, REG_DIRB, 0x00);
	struct timespec sleep = {0, 5*1000000};
	
	while(1) {
		
		primeCount = delayCalculator_getNumPrimesInLastSecond();
		displayCount();
		driveToPin(GPIO_44_PIN, 0);
		driveToPin(GPIO_61_PIN, 0);
		
		writeI2cRegister(i2cFileDesc, REG_OUTA, segments14Left);
		writeI2cRegister(i2cFileDesc, REG_OUTB, segments15Left);

		driveToPin(GPIO_61_PIN, 1);
		nanosleep(&sleep, (struct timespec *) NULL);
		driveToPin(GPIO_61_PIN, 0);
		driveToPin(GPIO_44_PIN, 0);

		writeI2cRegister(i2cFileDesc, REG_OUTA, segments14Right);
		writeI2cRegister(i2cFileDesc, REG_OUTB, segments15Right);

		driveToPin(GPIO_44_PIN, 1);
		nanosleep(&sleep, (struct timespec *) NULL);

		
		if(!PrimeFinder_isCalculating()) {
			driveToPin(GPIO_44_PIN, 0);
			driveToPin(GPIO_61_PIN, 0);
			break;
		}
	}

	close(i2cFileDesc);

	return NULL;
}