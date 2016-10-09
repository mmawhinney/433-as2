#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "primeFinder.h"
#include "udpListener.h"
#include "delayCalculator.h"

#define START_COUNTER 5000000000
#define MAX_BUFFER 1024

void readFromPipe(int readFileDesc) {
	FILE *readStream = fdopen(readFileDesc, "r");

	char buffer[MAX_BUFFER];
	while(!feof(readStream) && !ferror(readStream) && fgets(buffer, sizeof(buffer), readStream) != NULL) {
		printf("Prime Found: %s\n", buffer);
	}
	close(readFileDesc);
	fclose(readStream);
}

int main() {

	int fds[2];
	pipe(fds);

	delayCalculator_enableCape();
	int *initDelay = malloc(sizeof(int));
	delayCalculator_getReading(initDelay);
	int delay = delayCalculator_determineDelay(*initDelay);
    ThreadArgs arguments = { fds[1], START_COUNTER, delay };
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, &PrimeFinder_launchThread, (void*) &arguments);
	pthread_create(&tid2, NULL, &udpListener_launchThread, NULL);

	readFromPipe(fds[0]);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

	return 0;
}