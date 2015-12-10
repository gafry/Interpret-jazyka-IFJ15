#ifndef VYRAZY_H_EXP
#define VYRAZY_H_EXP

#include <stdlib.h>
#include <string.h>

#include "alokator.h"
#include "random.h"
#include "ial.h"
#include "scaner.h"
#include "data.h"

#define RADKY 14
#define SLOUPCE 14

typedef enum {
	PLUS       = 0,		// +
	MINUS      = 1,		// -
	KRAT       = 2,		// *
	DELENO     = 3,		// /
	ROVNASE	   = 4,		// ==
	NEROVNASE  = 5,		// !=
	MENSIROVNO = 6,		// <=
	VETSIROVNO = 7,		// >=
	MENSI      = 8,		// <
	VETSI      = 9,		// >
	LKULZAV    = 10,	// (
	PKULZAV    = 11,	// )
	ID     	   = 12,	// ;
	DOLAR      = 13,	// konstanta nebo nazev promenne
} tTabOperace;

typedef enum
{
	V = 0,	//>
	M = 1,	//<
	R = 2,	//=
	C = 3	//syntakticka chyba
} tTab;

const tTab precedencni_tabulka[RADKY][SLOUPCE];

typedef struct tSAData {
	int znamenko;
	char *nazev;
	tTabOperace typ;
	bool E;
} *tSADataPtr;

typedef struct tPrvek {
	tSADataPtr data;
	struct tPrvek *lptr;
	struct tPrvek *rptr;
} *tPrvekPtr;

typedef struct {
	tPrvekPtr vrchol;
	tPrvekPtr Act;
} tZasobnik;

tError expr(char **last);

tSADataPtr ResolveToken (tToken token, int i);
void VInit(tZasobnik *zasobnik);
int VEmpty(tZasobnik *zasobnik);
void VPush(tZasobnik *zasobnik, tSADataPtr data);
void VPop(tZasobnik *zasobnik);
void VPopZavorka(tZasobnik *zasobnik);
void VClear(tZasobnik *zasobnik);

tSADataPtr findNonE (tZasobnik *zasobnik);
tPrvekPtr findSth (tZasobnik *zasobnik, tTabOperace typ);
void makeInstr (tZasobnik *zasobnik);

char *newInt(tTabulka* halda, char *hodnota);
char *newDouble(tTabulka* halda, char *hodnota);
char *newStr(tTabulka* halda, char *hodnota);
char *newUnkown(tTabulka* halda);

#endif /* VYRAZY_H_EXP */
