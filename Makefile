RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic


prekladac: alokator.o lexikalni_analyzator.o main.o
	$(CC) alokator.o lexikalni_analyzator.o main.o -lm -o prekladac

clean:
	$(RM) *.o
