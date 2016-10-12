#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "primeFinder.h"
#include "udpListener.h"
#include "delayCalculator.h"
#include "primeCountDisplay.h"

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

    ThreadArgs arguments = { fds[1], START_COUNTER};
	pthread_t tid1, tid2, tid3, tid4;
	pthread_create(&tid1, NULL, &PrimeFinder_launchThread, (void*) &arguments);
	pthread_create(&tid2, NULL, &udpListener_launchThread, NULL);
	pthread_create(&tid4, NULL, &primeCountDisplay_launchThread, NULL);
	pthread_create(&tid3, NULL, &delayCalculator_launchThread, NULL);
	

	readFromPipe(fds[0]);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

	return 0;
}