/*
 * Implementace interpretu imperativního jazyka IFJ15
 *
 * Zadání: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2015.pdf
 *
 * Tým 094, varianta b/3/II:

 * Jakub Menšík - vedoucí (xmensi03)
 * Vojtěch Měchura (xmechu00)
 * Matěj Moravec (xmorav32)
 * Jan Morávek (xmorav33)
 * Jan Svoboda (xsvobo0u)
 *
 */

#ifndef STACK_H_ZASOBNIK
#define STACK_H_ZASOBNIK

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "alokator.h"
#include "ial.h"

typedef struct tStackPrvek {
	tTabulka *tab;
	int ramec;
	struct tStackPrvek *next;
} *tStackPrvekPtr;

typedef struct {
	tStackPrvekPtr vrchol;
} tStack;

void ZInit(tStack *zasobnik);
int ZEmpty(tStack *zasobnik);
void ZPush(tStack *zasobnik, tTabulka *tab, int ramec);
void ZPop(tStack *zasobnik, int i);
tTRPPolozka *ZSearch(tStack *zasobnik, char *nazev);
tTRPPolozka *ZRekurzeSearch(tStack *zasobnik, char *nazev);

#endif /* STACK_H_ZASOBNIK */