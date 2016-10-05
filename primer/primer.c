#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "primeFinder.h"

#define START_COUNTER 5000000000

void readFromPipe(int readFileDesc) {
	FILE *readStream = fdopen(readFileDesc, "r");

	char buffer[1024];
	while(!feof(readStream) && !ferror(readStream) && fgets(buffer, sizeof(buffer), readStream) != NULL) {
		printf("Prime Found: %s\n", buffer);
	}
	close(readFileDesc);
	fclose(readStream);
}

int main() {

	int fds[2];
	pipe(fds);

    ThreadArgs arguments = { fds[1], START_COUNTER };
	pthread_t tid;
	pthread_create(&tid, NULL, &PrimeFinder_launchThread, (void*) &arguments);

	readFromPipe(fds[0]);
    
    pthread_join(tid, NULL);

	return 0;
}