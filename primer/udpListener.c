#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "primeFinder.h"

#define PORT 12345
#define MAX_BUFFER 1024

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


void stopCalculating() {
	PrimeFinder_stopCalculating();
}

void handleGet(char *reply, char *tokens[]) {
	char *index = tokens[1];
	unsigned int idx = atoi(index);
	unsigned long long prime = getPrime(idx);
	if(prime != 0) {
		snprintf(reply, MAX_BUFFER, "Prime %u = %llu\n", idx, prime);
	} else {
		snprintf(reply, MAX_BUFFER, "Invalid argument. Must be between 1 and %u\n", primeCount());
	}
}

void handleLast(char *reply, char *tokens[]) {
	char *count = tokens[1];
	unsigned int counter = atoi(count);
	char buffer[MAX_BUFFER];
	if(counter > 50) {
		counter = 50;
		sprintf(buffer, "Only displaying last 50 primes = \n");
	} else {
		sprintf(buffer, "Last %d primes =\n", counter);	
	}
	strcat(reply, buffer);
	unsigned int numPrimes = primeCount();
	
	for(int i = numPrimes - counter; i < numPrimes; i++) {
		sprintf(buffer, "%llu\n", getPrime(i));
		strcat(reply, buffer);
	}
}

void handleFirst(char *reply, char *tokens[]) {
	char *count = tokens[1];
	unsigned int counter = atoi(count);
	char buffer[MAX_BUFFER];
	if(counter > 50) {
		counter = 50;
		sprintf(buffer, "Only displaying first 50 primes = \n");
	} else {
		sprintf(buffer, "First %d primes = \n", counter);	
	}
	strcat(reply, buffer);

	for(int i = 1; i <= counter; i++) {
		sprintf(buffer, "%llu\n", getPrime(i));
		strcat(reply, buffer);
	}
}

void* udpListener_launchThread(void *args) {
	char message[MAX_BUFFER];

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);

	int socketDesc = socket(PF_INET, SOCK_DGRAM, 0);

	bind(socketDesc, (struct sockaddr*) &sin, sizeof(sin));

	char *reply;
	char **tokens;

	while(1) {
		unsigned int sin_len = sizeof(sin);
		int bytesRecv = recvfrom(socketDesc, message, MAX_BUFFER, 0, (struct sockaddr*) &sin, &sin_len);

		message[bytesRecv-1] = 0;

		reply = malloc(sizeof(*reply) * MAX_BUFFER);
		memset(reply, 0, MAX_BUFFER);
		tokens = malloc(sizeof(char) * MAX_BUFFER);
		char *token = strtok(message, " ");
		int counter = 0;
		while(token) {
			tokens[counter] = token;
			token = strtok(NULL, " ");
			counter++;
		}

		
		if(counter == 0) {
			snprintf(reply, MAX_BUFFER, "%s is an invalid command!\n", message);
		} else if(strcmp(message, "help") == 0) {
			printHelp(reply);
		} else if(strcmp(message, "stop") == 0) {
			stopCalculating();
			snprintf(reply, MAX_BUFFER, "Program terminating...\n");
		} else if(strcmp(message, "count") == 0) {
			unsigned int count = primeCount();
			snprintf(reply, MAX_BUFFER, "%u", count);
		} else if(strcmp(tokens[0], "get") == 0 && counter == 2) {
			handleGet(reply, tokens);
		} else if(strcmp(tokens[0], "last") == 0 && counter == 2) {
			handleLast(reply, tokens);
		} else if(strcmp(tokens[0], "first") == 0 && counter == 2) {
			handleFirst(reply, tokens);
		} else {
			snprintf(reply, MAX_BUFFER, "%s is an invalid command!\n", message);
		}
		
		sprintf(message, "%s\n", reply);
		
		sin_len = sizeof(sin);
		sendto(socketDesc, message, strlen(message), 0, (struct sockaddr *) &sin, sin_len);
		
		free(reply);
		free(tokens);

		if(!PrimeFinder_isCalculating()) {
			break;
		}
	}
	close(socketDesc);

	return NULL;
}

