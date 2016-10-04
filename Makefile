

CC=arm-linux-gnueabihf-gcc
CFLAGS= -std=c99 -Wall -Werror -g -D _POSIX_C_SOURCE=200809L

%.o : %.c
	$(CC) -c $(CFLAGS) $<

all: noworky
	cp noworky ~/cmpt433/public/myApps/noworky
