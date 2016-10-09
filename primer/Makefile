# arm-linux-gnueabihf-gcc
program_NAME := primer
program_C_SRC := $(wildcard *.c)
program_C_OBJS := ${program_C_SRC:.c=.o}
program_OBJS := $(program_C_OBJS)

CC=arm-linux-gnueabihf-gcc
CFLAGS= -std=c99 -Wall -Werror -g -D _POSIX_C_SOURCE=200809L 
LDFLAGS=-lm -pthread

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(CC) $(program_OBJS) -o $(program_NAME) $(LDFLAGS)
	cp primer ~/cmpt433/public/myApps/primer

clean:
	-rm *.o
# %.o : %.c
	# $(CC) -c $(CFLAGS) $<

# primer: primer.o
	# $(CC) -o primer primer.o $(LDFLAGS)

# primer: primer.o
	# $(CC) $(CFLAGS) -o primer primer.o $(LDLIBS)

# primer.o: primer.c
	# $(CC) $(CFLAGS) -c primer.c -o primer.o $(LDLIBS)


	

# cp primer ~/cmpt433/public/myApps/primer
