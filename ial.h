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

extern int TRPVELIKOST; //velikost tabulky
extern tTabulka *halda;	//pomocne promenne

// funkce tabulky
int hash(tKlic klic);
void TRPInit(tTabulka *ptr);
tTRPPolozka *TRPSearch(tTabulka *ptr, tKlic klic);
void TRPInsert(tTabulka *ptr, tKlic klic, tData *data);
tData *TRPData(tTabulka *ptr, tKlic klic);
void TRPCopy(tTabulka *tabFull, tTabulka *tabEmpty);
void TRPDelete(tTabulka *tab);

// shell sort a boyer-moore
void shellSort(char *s1);
int boyer_moore (char *s, char *search);

#endif /* IAL_H_TABULKADATA */