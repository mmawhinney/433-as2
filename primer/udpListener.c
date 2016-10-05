#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "primeFinder.h"

#define PORT 12345


void printHelp() {
	printf("Accepted Commands:\n");
	printf("count 		-- show # of primes found\n");
	printf("get <int>	-- display prime # <int>\n");
	printf("first <int>	-- display first <int> primes found\n");
	printf("last <int>	-- display last <int> primes found\n");
	printf("stop 		-- stop calculating primes\n");
}

void primeCount() {

}

unsigned long long getPrime(int index) {
	return PrimeFinder_getPrimeByIndex(index);
}

void lastPrimes(int count) {

}

void firstPrimes(int count) {

}

void stopCalculating() {
	PrimeFinder_stopCalculating();
}

void* udpListener_launchThread(void *args) {
	char message[1024];

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);

	int socketDesc = socket(PF_INET, SOCK_DGRAM, 0);

	bind(socketDesc, (struct sockaddr*) &sin, sizeof(sin));

	while(1) {
		unsigned int sin_len = sizeof(sin);
		int bytesRecv = recvfrom(socketDesc, message, 1024, 0, (struct sockaddr*) &sin, &sin_len);

		message[bytesRecv] = 0;

		if(strcmp(message, "help\0\0") == 0) {
			printHelp();
		} else {
			printf("Invalid command!\n");
		}
		printf("bytes: %d\n", bytesRecv);
		printf("message: %s\n", message);
	}

	close(socketDesc);

	return NULL;
}

