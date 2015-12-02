#ifndef DATA_H_STRUKTURY
#define DATA_H_STRUKTURY

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alokator.h"
#include "ial.h"

//instrukce

typedef enum {
	I_START,
	I_LABEL,
	I_KONEC,
	I_PUSH_FRAME,
	I_POP_FRAME,
	I_POP_ALL,
	I_COUT,
	I_CIN,
	I_ADD,
	I_SUB,
	I_DIV,
	I_MUL,
	I_PRIR,
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
	tInstrElemPtr Last;
} tInstrList;

//parametry

typedef struct tParamElem {
	tData pData;
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

//funkce tabulky
void TSInit (tTSList *TS);
void TSInsertLast (tTSList *TS, tTabulka *tab);

/*void DLInitList (tDLList *L);
void DLInsertFirst (tDLList *L, int val);
void DLInsertLast(tDLList *L, int val);
void DLFirst (tDLList *L);
void DLLast (tDLList *L);
void DLCopyFirst (tDLList *L, int *val);
void DLCopyLast (tDLList *L, int *val);
void DLPostInsert (tDLList *L, int val);
void DLPreInsert (tDLList *L, int val);
void DLSucc (tDLList *L);
void DLPred (tDLList *L);
int DLActive (tDLList *L);*/

#endif /* DATA_H_STRUKTURY */