RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic


prekladac: alokator.o random_name.o ial.o data.o scaner.o syntax.o main.o
	$(CC) alokator.o random_name.o ial.o data.o scaner.o syntax.o main.o -lm -o prekladac

clean:
	$(RM)   *.o
