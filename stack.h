#ifndef STACK_H_ZASOBNIK
#define STACK_H_ZASOBNIK

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "alokator.h"
#include "ial.h"

typedef struct tPrvek {
	tTabulka *tab;
	bool boss;
	struct tPrvek *next;
} *tPrvekPtr;

typedef struct {
	tPrvekPtr vrchol;
} tZasobnik;

void ZInit(tZasobnik *zasobnik);
int ZEmpty(tZasobnik *zasobnik);
void ZPush(tZasobnik *zasobnik, tTabulka *tab, bool boss);
void ZPop(tZasobnik *zasobnik);
void ZTop(tZasobnik *zasobnik, tTabulka *tab);
void ZTopPop(tZasobnik *zasobnik, tTabulka *tab);
void ZClear(tZasobnik *zasobnik);
tTRPPolozka *ZSearch(tZasobnik *zasobnik, char *nazev);

#endif /* STACK_H_ZASOBNIK */