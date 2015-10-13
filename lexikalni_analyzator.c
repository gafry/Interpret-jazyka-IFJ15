#include <ctype.h>                  // OBSAHUJE FUNKCE PRO PRACI SE ZNAKY
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexikalni_analyzator.h"   // NAS HLAVICKOVY SOUBOR
#include "errors.h"

/*
  * KONSTANTA
  */

#define pocet_klicu 10

/*
 * KLICOVA SLOVA
 */

const char *klicSlova[pocet_klicu] =
{
"auto/0",
"cin/0",
"cout/0",
"double/0",
"else/0",
"for/0",
"if/0",
"int/0",
"return/0",
"string/0"
};

/*
 * GLOBALNI PROMENA
 */

 tToken token;
 int error;

 /*
  * PROTOTYPY STATICKYCH FUNKCI
  */

void vlozZnak(int znak, int *pocitadlo);
void vloztStav(tStav stav);
void vratZnak(int znak);
tStav zjistiIdent (char *slovo);
bool zkushexa (int c);

/*
 * FUNKCE
 */

tToken getToken(void) // NENI STATIC PROTOZE NEPOTREBUJEME ABY SI PAMATOVALA SVE PROMENNE
{
   /*
    * PRIPRAVA PROMENYCH PRO AUTOMAT
    */

   tStav stav = S_START;    // NASTACENI STAVU
   bool pokracovat = true;  // PODMINKA OKRACOVANI CYKLU AUTOMATU
   int pocitadlo = 0;       // POCET NACTENYCH ZNAKU
   int c;                   // NACITANY ZNAK


   /*
    * INICIALIZACE TOKENU
    */

   token.stav = S_START;
   token.data = NULL;

   /*
    * BEH AUTOMATU
    */

   while (pokracovat && (c = getc(soubor)))
   {
       switch(stav)
       {
       case S_START:        //POCATECNI STAV 00, NACTENI PRVNIHO SYMBOLU TOKENU A PODLE TOHO O JAK TYP TOKENU SE JEDNA
        {
            if ((isalpha(c)||(c=='_'))){
                    stav = S_IDENT;
            }
            else if (isdigit(c)){
                    stav = S_INT;
            }
            else if (c=='"'){
                    stav = S_TYPSTRING;
            }
            else if (c==','){
                    stav = S_CARKA;
            }
            else if (c=='.'){
                    stav = S_TECKA;
            }
            else if (c==';'){
                    stav = S_STREDNIK;
            }
            else if (c=='('){
                        stav = S_LZ;
            }
            else if (c==')'){
                stav = S_PZ;
            }
            else if (c=='{'){
                    stav = S_SLZ;
            }
            else if (c=='}'){
                    stav = S_SPZ;
            }
            else if (c=='+'){
                    stav = S_ADD;
            }
            else if (c=='/'){
                    stav = S_DIV;
            }
            else if (c=='<'){
                    stav = S_MEN;
            }
            else if (c=='*'){
                    stav = S_MUL;
            }
            else if (c=='='){
                    stav = S_ROV;
            }
            else if (c=='!'){
                    stav = S_VYKR;
            }
            else if (c=='-'){
                    stav = S_SUB;
            }
            else if (isdigit(c)&&(c<0)){    // EOF VRACI ZAORNE CISLA
                    stav = S_EOF;
            }
            else if (isspace(c)){       //NECHCEME JI ULOZIT DO TOKENU
                    stav = S_START;
            }
            else {
                    stav = S_CHYBAVS;
            }
            vlozZnak(c,&pocitadlo);         //ULOZIME ZNAK DO TOKENU
        break;
        }

       case S_IDENT:
        {
            if((isalpha(c))||(isdigit(c))||(c=='-')){
               vlozZnak(c,&pocitadlo);
               }
            // POKUD I DALSI ZNAK VYHOVUJE PODMINCE IDENTIFIKATORU, CTEME DALSI SYMBOL
            else {//POKUD DALSI SYMBOL NEVYHOVUJE PODMINCE, NACETLI JSME CELY IDENTIFIKATOR
                vloztStav(zjistiIdent(token.data)); //ZJISTI ZDA SE JEDNA O KLICOVE SLOVO NEBO
                stav = S_KONEC;                     // JINE KLICOVE SLOVO
            }
        break;
        }

       case S_TYPINT:
        {
            if(isdigit(c)){
                    vlozZnak(c, &pocitadlo);
            } // DALSI ZNAK JE CISLO, TAK TOKEN STALE BEREME JAKO INT
            else if (c=='.'){
                    stav = S_DPOM; vlozZnak(c, &pocitadlo);
            } // ODTED CISLO BEREME JAKO DESETINNE TYPU DOUBLE
            else if ((c=='e')||(c=='E')){
                    stav = S_DEXP; vlozZnak(c, &pocitadlo);
            } // ODTED BEREME CISLO JAKO CISLO S EXP TYPU DOUBLE
            else{
                    vloztStav(stav); stav = S_KONEC;
            } // POKUD DALSI ZNAK NEVYHOVUJE PODMINCE, DOCETLI JSME CELE CISLO, KTERE JE TYPU INT
        break;
        }

       case S_DPOM:
        {
            if(isdigit(c)){
                stav = S_DPOM;
                vlozZnak(c, &pocitadlo);
            }
            else if ((c=='e')||(c=='E')){
                    stav = S_DEXP; vlozZnak(c, &pocitadlo);
            } //ODTED BEREME CISLO JAKO CISLO S EXP TYPU DOUBLE
            else{
                    stav = S_TYPDOUBLE;
            }
        break;
        }
       case S_DEXP:
        {
            if ((c=='+')||(c=='-')) {
                stav = S_DEXPP;
                vlozZnak(c, &pocitadlo);
            }
        break;
        }

       case S_DEXP1:
        {
            if(isdigit(c)){
                stav = S_DEXPP;
                vlozZnak(c, &pocitadlo);
            }
            else {
                stav = S_CHYBAVS;
                vratZnak(c);
            }
        break;
        }

       case S_DEXPP:
        {
            if(isdigit(c)){
                stav = S_DEXPP;
                vlozZnak(c, &pocitadlo);
            }
            else  {
                stav = S_TYPDOUBLE;
            }
        break;
        }
       case S_ADD:
        {
            if (c=='+'){
                vlozZnak(c, &pocitadlo);
                stav = S_INK;       //DVE PO SOBE JDOUCI PLUS ZNACI INKREMENTACI
            }
            else {
                vloztStav(stav);   //POUZE JEDNO PLUS ZNACI FUNKCI SCTANI
                stav = S_KONEC;
            }
        break;
        }
       case S_DIV:
        {
            if (c=='*') {
                vlozZnak(c, &pocitadlo);
                stav = S_LKOM;       // SYMBOLY /* ZANC ZACATEK BLOKU KOENTARE
            }
            else if (c=='/') {
                vlozZnak(c, &pocitadlo);
                stav = S_KOMENT;     // SYMBOLY // ZNACI ZACATEK RADKOVEHO KOMENTARE
            }
            else if (c=='x')
            {
                vlozZnak(c, &pocitadlo);
                stav = S_HEXADECIMAL;
            }
            else if (c=='n')
            {
                stav = S_NOVYRADEK;
            }
            else {
                vloztStav(stav);   // POUZE JEDNO LOMITKO ZNACI OPERACI DELENI
                vratZnak((char)c);
            }
        break;
        }

       case S_HEXADECIMAL:
        {
            if (isdigit(c)||(isalpha(c)&&zkushexa(c)))
            {
                vlozZnak(c, &pocitadlo);
            }
            else{
                vloztStav(stav);
                stav = S_KONEC;
            }
        break;
        }

       case S_MEN:
        {
            if (c=='<'){
                vlozZnak(c, &pocitadlo);
                stav = S_LPRIR;
            }
            else if (c=='='){
                vlozZnak(c, &pocitadlo);
                stav = S_MENROV;
            }
            else {
                vloztStav(stav);
                stav = S_KONEC;
            }
        break;
        }

       case S_MUL:
        {
           if (c=='/'){
                vlozZnak(c, &pocitadlo);
                stav = S_PKOM;
           }
           else {
            vloztStav(stav);
            stav = S_KONEC;
           }
        break;
        }

       case S_PRIR:
        {
            if (c=='='){
                vlozZnak(c, &pocitadlo);
                stav = S_ROV;
            }
            else {
                vloztStav(stav);
                stav = S_KONEC;
            }
        break;
        }

       case S_SUB:
        {
            if (c=='-'){
                stav = S_DEK;
            }
            else {
                vloztStav(stav);
                stav = S_KONEC;
            }
        break;
        }

       case S_VEC:
        {
            if (c=='>'){
                vlozZnak(c, &pocitadlo);
                stav = S_PPRIR;
            }
            else {
                vloztStav(stav);
                stav = S_KONEC;
            }
        break;
        }

       case S_VYKR:
        {
            if (c=='='){
                stav = S_NEROV;
            }
            else {
                stav = S_CHYBAVS;
            }
        break;
        }

       case S_TYPSTRING:
        {
            if (c!='"'){
                vlozZnak(c, &pocitadlo);
            }
            else {
                vloztStav(stav);
                stav = S_KONEC;
            }
        }
        /*
         * PREDKONECNE STAVY
         */

       case S_KLIC:
       case S_TYPDOUBLE:
       case S_AUTO:
       case S_CIN:
       case S_COUT:
       case S_DOUBLE:
       case S_ELSE:
       case S_FOR:
       case S_IF:
       case S_INT:
       case S_STRING:
       case S_RETURN:
       case S_CARKA:
       case S_KOMENT:
       case S_LKOM:
       case S_PKOM:
       case S_LPRIR:
       case S_PPRIR:
       case S_TECKA:
       case S_STREDNIK:
       case S_LZ:
       case S_PZ:
       case S_SLZ:
       case S_SPZ:
       case S_NOVYRADEK:
       case S_DEK:
       case S_INK:
       case S_ROV:
       case S_NEROV:
       case S_MENROV:
       case S_VECROV:
       case S_EOF:
        {
            vloztStav(stav); //NASTAVÍ STAV TOKENY NA SPRAVNY STAV AUTOMATU
            stav = S_KONEC;
            break;
        }

        /*
         * KONECNE STAVY
         */

       case S_KONEC:
        {
            vratZnak((char)c);
            pokracovat = false;
            break;
        }
       case S_CHYBAVS:      // CHYBNY VSTUP
        {
            error = ERR_LEX;
            pokracovat = false;
            break;
        }
       case S_CHYBABE:      // CHYBA ZA BEHU
        {
            error = ERR_INT;
            pokracovat = false;
            break;
        }

       }
    pocitadlo++; //NA KONCI CYKLU INKREMENRUJEME POCITADLO PRO SPRAVNOU VELIKOST ALOKOVANE PAMETI PRO TOKEN
   }
    return token;
}

void vlozZnak(int znak, int *pocitadlo){
    if((token.data = (char *) realloc(token.data, (*pocitadlo) + 2)))
    {
        token.data[(*pocitadlo)] = znak;      // VLOZIME NACTENY ZNAK
        token.data[(*pocitadlo)+1] = '\0';    // ZA NACTENY ZNAK VLOZIME KONCOVY ZNAK
        (*pocitadlo)++;                       // INKREMENTUJEME POCITADLO ZNAKU
    }
    else error = ERR_OTH;
}

void vloztStav(tStav stav)
{
    token.stav = stav;
}

void vratZnak(int znak){
    if (!isspace(znak))  //POKUD NACTENY ZNAK NEPATRI DO TOKENU, TAK JEJ VRATIME ZPET
    {                 //POKUD TO NENI MEZERA
        ungetc(znak,soubor);
    }
}

tStav zjistiIdent(char *slovo){
    int pomprom;
    int pom = 20;
    tStav pomstav;
    for (pomprom = 0; pomprom<pocet_klicu; pomprom++){
        if (!(strcmp(slovo, klicSlova[pomprom])))   // MUSIME EGOVAT PROTOŽE STRCMP PRI ROVNOSTI VRAC NULU
        {
            pom = pomprom;
        }
    }
    if (pom == 0){
        pomstav = S_AUTO;
    }
    if (pom == 1){
        pomstav = S_CIN;
    }
    if (pom == 2){
        pomstav = S_COUT;
    }
    if (pom == 3){
        pomstav = S_DOUBLE;
    }
    if (pom == 4){
        pomstav = S_ELSE;
    }
    if (pom == 5){
        pomstav = S_FOR;
    }
    if (pom == 6){
        pomstav = S_IF;
    }
    if (pom == 7){
        pomstav = S_INT;
    }
    if (pom == 8){
        pomstav = S_RETURN;
    }
    if (pom == 9){
        pomstav = S_STRING;
    }
    if (pom == 20){
        pomstav = S_IDENT;
    }
    return pomstav;
}

bool zkushexa (int c){
int table [12] = {'a','b','c','d','e','f','A','B','C','D','E','F'};
int counter = 0;
bool hexa = false;
while (counter<12){
    if (c==table[counter])
    {
        hexa = true;
    }
    counter++;
}
return hexa;
}
