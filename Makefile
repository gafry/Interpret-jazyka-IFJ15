RM=rm
CC=g++
CFLAGS=-c -Wall -std=c++11 -O3 -pedantic


prekladac: alokator.o lexikalni_analyzator.o main.o
	$(CC) alokator.o lexikalni_analyzator.o main.o -lm -o prekladac

clean:
	$(RM)   *.o
