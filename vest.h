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

#ifndef VEST_H_FUNKCE
#define VEST_H_FUNKCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alokator.h"
#include "scaner.h"
#include "data.h"
#include "vyrazy.h"
#include "ial.h"

tError vest(int i);

char *concatVest(char *s1, char *s2);
int lengthVest(char *s);
char *sortVest(char *s1);
char *substrVest(char *s1, int i1, int i2);

#endif /* VEST_H_FUNKCE */