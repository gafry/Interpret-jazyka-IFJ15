#ifndef IAL_H_TABULKADATA
#define IAL_H_TABULKADATA

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "alokator.h"

#define TRP_VELIKOST 13

typedef char *tKlic;

typedef union {
	int i;
	double d;
	char *s;
	void *label;
	void *tabulka;
	void *args;
} tHodnota;

typedef struct tdata {
	//1 int
	//2 double
	//3 string
	//4 label
	//5 tabulka
	//6 seznam parametru
	//7 auto
	int typ;
	tHodnota *hodnota;
	char *nazev;
	bool def;
	//0 neni to ramec
	//1 hlavni ramec - funkce
	//2 zacatek vnoreneho bloku
	//3 random shit
	int ramec;
} tData;

typedef struct tTRPpolozka {
	tData *data;
	tKlic klic;
	struct tTRPpolozka *next;
} tTRPPolozka;

typedef tTRPPolozka *tTabulka[TRP_VELIKOST];

extern int TRPVELIKOST;
extern tTabulka *halda;

int hash(tKlic klic);
void TRPInit(tTabulka *ptr);
tTRPPolozka *TRPSearch(tTabulka *ptr, tKlic klic);
void TRPInsert(tTabulka *ptr, tKlic klic, tData *data);
tData *TRPData(tTabulka *ptr, tKlic klic);
void TRPCopy(tTabulka *tabFull, tTabulka *tabEmpty);
void TRPDelete(tTabulka *tab);

#endif /* IAL_H_TABULKADATA */