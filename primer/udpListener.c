#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "primeFinder.h"

#define PORT 12345


void printHelp(char *help) {
	strcat(help, "Accepted Commands:\n");
	strcat(help, "count 		-- show # of primes found\n");
	strcat(help, "get <int>	-- display prime # <int>\n");
	strcat(help, "first <int>	-- display first <int> primes found\n");
	strcat(help, "last <int>	-- display last <int> primes found\n");
	strcat(help, "stop 		-- stop calculating primes\n");
}

unsigned int primeCount() {
	return PrimeFinder_getNumPrimesFound();
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

		message[bytesRecv-1] = 0;

		char *tokens[1024];
		char *token = strtok(message, " ");
		int counter = 0;
		while(token) {
			tokens[counter] = token;
			token = strtok(NULL, " ");
			counter++;
		}

		char reply[1024];
		if(strcmp(message, "help") == 0) {
			printHelp(reply);
		} else if(strcmp(message, "stop") == 0) {
			stopCalculating();
			snprintf(reply, 1024, "Program terminating...\n");
		} else if(strcmp(message, "count") == 0) {
			unsigned int count = primeCount();
			snprintf(reply, 1024, "%u", count);
		} else if(strcmp(tokens[0], "get") == 0) {
			char *index = tokens[1];
			unsigned int idx = atoi(index);
			unsigned long long prime = getPrime(idx);
			if(prime != 0) {
				snprintf(reply, 1024, "%llu\n", prime);
			} else {
				snprintf(reply, 1024, "Invalid argument. Must be between 1 and %u\n", primeCount());
			}
			
		// } else if() {
		// 	lastPrimes();
		// } else if() {
		// 	firstPrimes();
		// }
		} else {
			printf("%s is an invalid command!\n", message);
		}

		sprintf(message, "%s\n", reply);

		sin_len = sizeof(sin);
		sendto(socketDesc, message, strlen(message), 0, (struct sockaddr *) &sin, sin_len);
	}

	close(socketDesc);

	return NULL;
}

