#ifndef IAL_H_TABULKADATA
#define IAL_H_TABULKADATA

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include "data.h"
#include "alokator.h"

#define TRP_VELIKOST 101

typedef char * tKlic;

typedef union {
	int i;
	double d;
	char *s;
	void *label;
	//tInstrElemPtr label;
} tHodnota;

typedef struct tdata {
	//1 int
	//2 double
	//3 string
	//4 label
	int typ;
	tHodnota *hodnota;
	char *nazev;
	bool def;
} tData;

typedef struct tTRPpolozka {
	tData *data;
	tKlic klic;
	struct tTRPpolozka *next;
} tTRPPolozka;

typedef tTRPPolozka *tTabulka[TRP_VELIKOST];

extern int TRPVELIKOST;

int hash(tKlic klic);
void TRPInit(tTabulka *ptr);
tTRPPolozka *TRPSearch(tTabulka *ptr, tKlic klic);
void TRPInsert(tTabulka *ptr, tKlic klic, tData *data);
tData *TRPData(tTabulka *ptr, tKlic klic);

#endif /* IAL_H_TABULKADATA */