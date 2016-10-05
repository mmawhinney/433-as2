#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "primeFinder.h"

void readFromPipe(int readFileDesc) {
	FILE *readStream = fdopen(readFileDesc, "r");

	char buffer[1024];
	while(!feof(readStream) && !ferror(readStream) && fgets(buffer, sizeof(buffer), readStream) != NULL) {
		printf("Prime Found: %s\n", buffer);
	}
}

int main() {

	int fds[2];
	pipe(fds);

	PrimeFinder_launchThread(fds[1]);

	readFromPipe(fds[0]);

	return 0;
}