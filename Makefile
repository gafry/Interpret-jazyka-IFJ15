RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic


prekladac: alokator.o random.o ial.o data.o stack.o scaner.o syntax.o interpret.o main.o
	$(CC) alokator.o random.o ial.o data.o stack.o scaner.o syntax.o interpret.o main.o -lm -o prekladac

clean:
	$(RM)   *.o
