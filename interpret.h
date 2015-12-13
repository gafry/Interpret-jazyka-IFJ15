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

#ifndef INTERPRET_H_HLAVA
#define INTERPRET_H_HLAVA
             
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h> 

#include "stack.h"
#include "data.h"
#include "ial.h"
#include "alokator.h"
#include "vyrazy.h"
#include "vest.h"

tError preloz();

#endif /* INTERPRET_H_HLAVA */