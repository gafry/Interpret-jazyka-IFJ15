#ifndef DATA_H_STRUKTURY
#define DATA_H_STRUKTURY

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alokator.h"
#include "ial.h"

//instrukce

typedef enum {
	I_START,			//0
	I_FOR,				//1
	I_KONEC,			//2
	I_PUSH_FRAME,		//3
	I_POP_FRAME,		//4
	I_POP_ALL,			//5
	I_COUT,				//6
	I_CIN,				//7
	I_IF,				//8
	I_ELSE,				//9
	I_IFELSE,			//10
	I_ADD,				//11
	I_SUB,				//12
	I_DIV,				//13
	I_MUL,				//14
	I_PRIR,				//15
	I_VET,				//16
	I_MEN,				//17
	I_VETROV,			//18
	I_MENROV,			//19
	I_ROV,				//20
	I_NEROV,			//21
	I_EXP_LAST,			//22
	I_RETURN,			//23
	I_FOR_VYRAZ,		//24
	I_FOR_START,		//25
	I_FOR_KONEC,		//26
	I_KONEC_FUNKCE,		//27
	I_FUNKCE,			//28
	I_START_FUNKCE,		//29
} tITyp;

typedef struct {
	tITyp op;
	char *adr1;
	char *adr2;
	char *vysl;
} tInstrukce;

typedef struct tInstrElem {
	tInstrukce instr;
	struct tInstrElem *jump;
	struct tInstrElem *lptr;
	struct tInstrElem *rptr;
} *tInstrElemPtr;

typedef struct {
	tInstrElemPtr First;
	tInstrElemPtr Act;
	tInstrElemPtr Pom;
	tInstrElemPtr Last;
} tInstrList;

//parametry

typedef struct tParamElem {
	tData *data;
	struct tParamElem *lptr;
	struct tParamElem *rptr;
} *tParamElemPtr;

typedef struct {
	tParamElemPtr First;
	tParamElemPtr Act;
	tParamElemPtr Last;
} tParamList;

//funkce

typedef struct tGlobalData {
	tParamElemPtr params;
	//tTabulka *lokProm;
	tInstrElemPtr paskaZ;
	tInstrElemPtr paskaK;
	tInstrElemPtr returnPtr;
	//1 int
	//2 double
	//3 string
	int typ;
	bool def;
	char *nazev;
} tGData, *tGDataPtr;

typedef struct tGElem {              
	tGData *funkce;                                          
	struct tGElem *lptr;        
	struct tGElem *rptr;        
} *tGElemPtr;

typedef struct {                              
    tGElemPtr First;                   
    tGElemPtr Act;                   
    tGElemPtr Last;             
} tGList;

extern tGList global;
         
//tabulky

typedef struct tTSElem {
	tTabulka *ts;
	struct tTSElem *lptr;
	struct tTSElem *rptr;
} *tTSElemPtr;

typedef struct {
	tTSElemPtr First;
	tTSElemPtr Act;
	tTSElemPtr Last;
} tTSList;

//funkce instrukci     
tInstrukce newInstr(tITyp op, char *adr1, char *adr2, char *vysl);
void ILInit (tInstrList *I);
void ILInsertLast (tInstrList *I, tInstrukce instr);
void ILFirst (tInstrList *I);

//funkce funkci
void GInit (tGList *G);
void GInsertLast (tGList *G, tGData *funkce);
void GFirst (tGList *G);
void GCopyMain (tGList *G, tInstrList *I);
bool GFind (tGList *G, char *name);
tGData *GFindData (tGList *G, char *name);

//funkce tabulky
void TSInit (tTSList *TS);
void TSInsertLast (tTSList *TS, tTabulka *tab);

//funkce parametru
void PInit (tParamList *P);
bool PCheck (tParamElemPtr par1, tParamElemPtr par2);
void PInsertTab (tParamElemPtr par, tTabulka *tab);
void PInsertLast (tParamList *P, int typ, char *nazev);
void PInsertArg (tParamList *P, int typ, char *nazev, int i, double d, char *s);

#endif /* DATA_H_STRUKTURY */