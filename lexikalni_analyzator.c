#include <ctype.h>                  // OBSAHUJE FUNKCE PRO PRACI SE ZNAKY
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexikalni_analyzator.h"   // NAS HLAVICKOVY SOUBOR
#include "alokator.h"
#include "str.h"
//#include "errors.h"

/*
  * KONSTANTA
  */

#define pocet_klicu 11
#define ERR_LEX -1
#define ERR_INT -2
#define ERR_OTH -3

/*
 * KLICOVA SLOVA
 */

const char *klicSlova[pocet_klicu] =
{
	"auto\0",
	"cin\0",
	"cout\0",
	"double\0",
	"else\0",
	"for\0",
	"if\0",
	"int\0",
	"main\0",
	"return\0",
	"string\0"
};

/*
 * GLOBALNI PROMENA
 */

 tToken token;
 int error;
 FILE * soubor;

 /*
  * PROTOTYPY STATICKYCH FUNKCI
  */

static void vlozZnak(int znak, int *pocitadlo);
static void vloztStav(tStav stav);
static void vratZnak(int znak, FILE* soubor);
static tStav zjistiIdent (char *slovo);



/*
 * FUNKCE
 */

 void setFile (FILE *f) {
 	soubor = f;
 }

tToken getToken() // NENI STATIC PROTOZE NEPOTREBUJEME ABY SI PAMATOVALA SVE PROMENNE
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
    * OTEVRENI SOUBORU
    */

    //soubor = fopen("txt.txt", "r");

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
                    vlozZnak(c,&pocitadlo);
            }
            else if (isdigit(c)){
                    stav = S_TYPINT;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='"'){
                    stav = S_TYPSTRING;
            }
            else if (c==','){
                    stav = S_CARKA;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='.'){
                    stav = S_TECKA;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c==';'){
                    stav = S_STREDNIK;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='('){
                    stav = S_LZ;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c==')'){
                	stav = S_PZ;
                	vlozZnak(c,&pocitadlo);
            }
            else if (c=='{'){
                    stav = S_SLZ;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='}'){
                    stav = S_SPZ;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='+'){
                    stav = S_ADD;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='/'){
                    stav = S_DIV;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='<'){
                    stav = S_MEN;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='*'){
                    stav = S_MUL;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='='){
                    stav = S_ROV;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='>'){
                    stav = S_VEC;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='!'){
                    stav = S_VYKR;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c=='-'){
                    stav = S_SUB;
                    vlozZnak(c,&pocitadlo);
            }
            else if (c==EOF){    // EOF VRACI ZAORNE CISLA
                    stav = S_EOF;
            }
            /*else if (isdigit(c)&&(c<0)){    // EOF VRACI ZAORNE CISLA
                    stav = S_EOF;
            }*/
            else if (isspace(c)){       //NECHCEME JI ULOZIT DO TOKENU
                    stav = S_START;
            }
            else {
                    stav = S_CHYBAVS;
            }
            
        break;
        }

       case S_IDENT:
        {
            if((isalpha(c))||(isdigit(c))||(c=='_')){
               vlozZnak(c,&pocitadlo);
            }
            										// POKUD I DALSI ZNAK VYHOVUJE PODMINCE IDENTIFIKATORU, CTEME DALSI SYMBOL
            else {									//POKUD DALSI SYMBOL NEVYHOVUJE PODMINCE, NACETLI JSME CELY IDENTIFIKATOR
                vloztStav(zjistiIdent(token.data)); //ZJISTI ZDA SE JEDNA O KLICOVE SLOVO NEBO
                stav = S_KONEC;                     // JINE KLICOVE SLOVO
                vratZnak(c,soubor);
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
                    vloztStav(stav); vratZnak(c,soubor); stav = S_KONEC;
            } // POKUD DALSI ZNAK NEVYHOVUJE PODMINCE, DOCETLI JSME CELE CISLO, KTERE JE TYPU INT
        break;
        }

       case S_DPOM:
        {
            if(isdigit(c)){
                stav = S_DPOM2;
                vlozZnak(c, &pocitadlo);
            }
            else {
                stav = S_CHYBAVS;
                vratZnak(c,soubor);
            }
        break;
        }
        case S_DPOM2:
        {
        	if (isdigit(c)){
        		vlozZnak(c, &pocitadlo);
        	} else if ((c=='e')||(c=='E')){
                stav = S_DEXP; vlozZnak(c, &pocitadlo);
            } else {
            	stav = S_TYPDOUBLE;
            	vratZnak(c, soubor);
            }
        	break;
        }
       case S_DEXP:
        {
            if ((c=='+')||(c=='-')) {
                stav = S_DEXP1;
                vlozZnak(c, &pocitadlo);
            } else if (isdigit(c)) {
            	stav = S_DEXPP;
            	vlozZnak(c, &pocitadlo);
            }
            else {
                vratZnak(c,soubor);
                stav = S_CHYBAVS;
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
                vratZnak(c, soubor);
            }
        break;
        }

       case S_DEXPP:
        {
            if(isdigit(c)){
                vlozZnak(c, &pocitadlo);
            }
            else  {
                vratZnak(c,soubor);
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
                vratZnak(c,soubor);
                vloztStav(stav);   //POUZE JEDNO PLUS ZNACI FUNKCI SCTANI
                stav = S_KONEC;
            }
        break;
        }
       case S_DIV:
        {
            if (c=='*') {
                token.data = NULL;
                stav = S_LKOM;       // SYMBOLY /* ZANC ZACATEK BLOKU KOENTARE
            }
            else if (c=='/') {
                token.data = NULL;
                stav = S_KOMENTP;     // SYMBOLY // ZNACI ZACATEK RADKOVEHO KOMENTARE
            }
            else {
                vratZnak(c,soubor);
                vloztStav(stav);   // POUZE JEDNO LOMITKO ZNACI OPERACI DELENI
                stav = S_KONEC;
            }
        break;
        }

       case S_KOMENTP:
        {
            if(c=='\n'){
                stav = S_START;
            }
        break;
        }

       case S_LKOM:
        {
            if (c=='*'){
                stav = S_PKOM;
            }
        break;
        }

       case S_PKOM:
        {
            if (c=='/') {
                stav = S_START;
            }
            else {
                stav = S_LKOM;
            }
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
                vratZnak(c,soubor);
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
                vratZnak(c,soubor);
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
                vratZnak(c,soubor);
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
                vratZnak(c,soubor);
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
                vratZnak(c,soubor);
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
        break;
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
       case S_MAIN:
       case S_STRING:
       case S_RETURN:
       case S_CARKA:
       case S_KOMENT:
       case S_LPRIR:
       case S_PPRIR:
       case S_TECKA:
       case S_STREDNIK:
       case S_LZ:
       case S_PZ:
       case S_SLZ:
       case S_SPZ:
       case S_MUL:
       case S_DEK:
       case S_INK:
       case S_ROV:
       case S_NEROV:
       case S_MENROV:
       case S_VECROV:
       case S_EOF:
        {
            vloztStav(stav); //NASTAV\CD STAV TOKENY NA SPRAVNY STAV AUTOMATU
            vratZnak(c, soubor);
            stav = S_KONEC;
            break;
        }

        /*
         * KONECNE STAVY
         */

       case S_KONEC:
        {
            vratZnak(c, soubor);
            pokracovat = false;
            break;
        }
       case S_CHYBAVS:      // CHYBNY VSTUP
        {
            vloztStav(stav);
            error = ERR_LEX;
            pokracovat = false;
            break;
        }
       case S_CHYBABE:      // CHYBA ZA BEHU
        {
            vloztStav(stav);
            error = ERR_INT;
            pokracovat = false;
            break;
        }

       }
   }
    return token;
}

static void vlozZnak(int znak, int *pocitadlo){
    if((token.data = (char *) newRealloc(token.data, (*pocitadlo) + 2)))
    {
    	token.data[(*pocitadlo)+1] = '\0';    // ZA NACTENY ZNAK VLOZIME KONCOVY ZNAK
        token.data[(*pocitadlo)] = znak;      // VLOZIME NACTENY ZNAK
        (*pocitadlo)++;                       // INKREMENTUJEME POCITADLO ZNAKU
    }
    else error = ERR_OTH;
}

static void vloztStav(tStav stav)
{
    token.stav = stav;
}

static void vratZnak(int znak, FILE* soubor){
    if (!isspace(znak))  //POKUD NACTENY ZNAK NEPATRI DO TOKENU, TAK JEJ VRATIME ZPET
    {                 //POKUD TO NENI MEZERA
        ungetc(znak,soubor);
    }
    //ungetc(znak,soubor);
}

static tStav zjistiIdent(char *slovo){
    int pomprom;
    int pom = 20;
    tStav pomstav;
    for (pomprom = 0; pomprom<pocet_klicu; pomprom++){
        if (!(strcmp(slovo, klicSlova[pomprom])))   // MUSIME EGOVAT PROTO\8EE STRCMP PRI ROVNOSTI VRAC NULU
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
    	pomstav = S_MAIN;
    }
    if (pom == 9){
        pomstav = S_RETURN;
    }
    if (pom == 10){
        pomstav = S_STRING;
    }
    if (pom == 20){
        pomstav = S_IDENT;
    }
    return pomstav;
}