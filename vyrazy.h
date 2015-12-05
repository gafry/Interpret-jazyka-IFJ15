#ifndef VYRAZY_H_EXP
#define VYRAZY_H_EXP

#include <stdlib.h>
#include <string.h>

#include "ial.h"
#include "data.h"
#include "alokator.h"
#include "random.h"
#include "scaner.h"

typedef struct tPrvek {
	int data;
	struct tPrvek *next;
} *tPrvekPtr;

typedef struct {
	tPrvekPtr vrchol;
} tZasobnik;

void ZInit(tZasobnik *zasobnik);
int ZEmpty(tZasobnik *zasobnik);
void ZPush(tZasobnik *zasobnik, int data);
void ZPop(tZasobnik *zasobnik);
void ZTop(tZasobnik *zasobnik, int *data);
void ZTopPop(tZasobnik *zasobnik, int *data);
void ZClear(tZasobnik *zasobnik);

char *newInt(tTabulka* halda, char *hodnota);
char *newDouble(tTabulka* halda, char *hodnota);
char *newStr(tTabulka* halda, char *hodnota);

bool IsKeyword(char *nazev);
void pom_vynuluj(tData *pom2, tData *pom3, tData *pom4);
tError TokenResolve(tToken token, tTokenResolved *t, int i);

#endif /* VYRAZY_H_EXP */