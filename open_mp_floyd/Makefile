#!/bin/bash

CC = gcc
files = main.c functions.c pfloyd.c
objects = main.o functions.o pfloyd.o
LIBS = -lm -lpthread -lrt -fopenmp
MAIN = floyd

CFLAGS = -W -Wall -Wextra

.PHONY: all
all: $(MAIN)
	@echo "Pthreads Floyd Algorithm Compiled Successfully!!!"

$(MAIN): $(objects)
	$(CC) $(CFLAGS) $(objects) $(LIBS) -o $@

functions.o		: functions.c functions.h globals.h
			  $(CC) -c -g functions.c

pfloyd.o		: pfloyd.c pfloyd.h functions.h globals.h
			  $(CC) -c -g pfloyd.c -fopenmp

main.o		: main.c pfloyd.h functions.h globals.h
			  $(CC) -c -g main.c

.PHONY: clean
clean:
	@echo "all object files will be deleted!"
	rm -f *.o *~ *.out
