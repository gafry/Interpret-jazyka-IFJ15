#include <stdlib.h>
#include <string.h>

#define TRP_VELIKOST 101

typedef char * tKlic;

typedef enum {
	tInt,
	tDouble,
	tString
} tTypPromenne;

typedef union {
	int i;
	double d;
	char *s;
} tHodnota;

typedef struct tdata {
	tTypPromenne promenna;
	tHodnota hodnota;
	char *nazev;
} tData;

typedef struct TTRPPolozka {
	tData data;
	tKlic klic;
	struct TTRPPolozka *next;
} tTRPPolozka;

typedef tTRPPolozka tTabulka[TRP_VELIKOST];

extern int TRPVELIKOST;

int hash(tKlic klic);
void TRPInit(tTabulka *ptr);
tTRPPolozka *TRPSearch(tTabulka *ptr, tKlic klic);
void TRPInsert(tTabulka *ptr, tKlic klic, tData data);
tData *TRPData(tTabulka *ptr, tKlic klic);