
program_NAME := primer
program_C_SRC := delayCalculator.c fileAccessor.c primer.c udpListener.c primeCountDisplay.c primeFinder.c
program_C_OBJS := ${program_C_SRC:.c=.o}
program_OBJS := $(program_C_OBJS)

CC=arm-linux-gnueabihf-gcc
CFLAGS= -std=c99 -Wall -Werror -g -D _POSIX_C_SOURCE=200809L 
LDFLAGS=-lm -pthread

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(CC) $(program_OBJS) -o $(program_NAME) $(LDFLAGS)
	cp primer ~/cmpt433/public/myApps/primer
	$(CC) $(CFLAGS) noworky.c -o noworky
	cp noworky ~/cmpt433/public/myApps/noworky

clean:
	-rm *.o

	
