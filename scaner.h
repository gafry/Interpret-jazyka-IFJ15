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

#ifndef SCANER_H_LEX
#define SCANER_H_LEX

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "alokator.h"

/*
* STAVY AUTOMATU
*/

typedef enum
{
    //RIDICI STAVY
    S_START,        //  0 START
    S_KONEC,        //  1 KONEC
    S_IDENT,        //  2 IDENTIFIKÁTOR
    S_KLIC,         //  3 KLICOVA SLOVA

    //CHYBOVE STAVY
    S_CHYBAVS,      //  4 CHYBA NA VSTUPU
    //S_CHYBABE,      //  5 CHYBA ZA BEHU

    // DATOVE TYPY
    S_TYPDOUBLE = 5,//  5
    S_TYPINT,       //  6
    S_TYPSTRING,    //  7
    S_TYPSTRINGD,   //  8
    S_TYPSTRINGX,   //  9

    //KLICOVA SLOVA
    S_AUTO = 10,    // 10 AUTO
    S_CIN,          // 11 CIN
    S_COUT,         // 12 COUT
    S_DOUBLE,       // 13 DOUBLE
    S_ELSE,         // 14 ELSE
    S_FOR,          // 15 FOR
    S_IF,           // 16 IF
    S_INT,          // 17 INT
    S_RETURN,       // 18 RETURN
    S_STRING,       // 19 STRING

    //ZNAKY
    S_CARKA,        // 20 ,
    S_LKOM,         // 21 /* + ZACATEK BLOKU KOMENTARU
    S_PKOM,         // 22 */ KONEC BLOKU KOMENTARU
    S_LPRIR,        // 23 <<
    S_PPRIR,        // 24 >>
    S_TECKA,        // 25 .     PRO DESETINNA CISLA
    S_STREDNIK,     // 26 ;
    S_LZ,           // 27 (
    S_PZ,           // 28 )
    S_SLZ,          // 29 {
    S_SPZ,          // 30 }

    //POMOCNE STAVY
    S_DPOM,         // 31 DOUBLE JAKO DESETINNE CISLO
    S_DEXP,         // 32 DOUBLE JAKO EXPONENT
    S_DEXP1,        // 33
    S_DEXPP,        // 34 DOUBLE JAKO EXPONENT POMOCNY
    S_DPOM2,  		// 35 PRO HEXADECIMALNI CISLA
    S_KOMENTP,      // 36 ZJISTI OBSAH RDKOVEHO KOMENTARE
    S_TYPSTRINGZ,      // 37
    S_TYPSTRINGA,      // 38

    //MATEMATICKE OPERACE
    S_ADD = 40,     // 40 +
    S_DIV,          // 41 /
    S_MEN,          // 42 <
    S_MUL,          // 43 *
    S_PRIR,         // 44 =
    S_ROV,          // 45 ==
    S_NEROV,        // 46 !=
    S_MENROV,       // 47 <=
    S_VECROV,       // 48 >=
    S_SUB,          // 49 -
    S_VEC,          // 50 >
    S_VYKR,         // 51 ! 

    S_EOF = 60      // 60 EOF
} tStav;

/*
*   STRUKTURA PRO TOKEN
*/

typedef struct
{
    tStav stav;
    char *data;
} tToken;

/*
* ZAJISTI ABY TOKEN BYL GLOBALNI PROMENNOU
*/

extern tToken token;

/*
* PROTOTYPY FUNKCI
*/

tToken getToken();  // VRATI TOKEN
void setFile (FILE *f);

/*static void vlozZnak(int znak, int *pocitadlo);
static void vloztStav(tStav stav);
static void vratZnak(int znak, FILE* soubor);
static tStav zjistiIdent (char *slovo);
//int esc(int c, tStav stav);*/

#endif /* SCANER_H_LEX */


