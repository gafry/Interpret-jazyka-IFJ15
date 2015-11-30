#ifndef DATA_H_STRUKTURY
#define DATA_H_STRUKTURY

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alokator.h"
#include "ial.h"

//instrukce

typedef enum {
	I_LABEL,
	I_KONEC,
	I_RAMEC,
	I_COUT,
	I_CIN,
} tITyp;

typedef struct {
	tITyp op;
	tTRPPolozka *adr1;
	tTRPPolozka *adr2;
	tTRPPolozka *vysl;
} tInstrukce;

typedef struct tInstrElem {
	tInstrukce instr;
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
	tTabulka *lokProm;
	tInstrElemPtr paska;
	//return
	bool def;
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
         
//funkce instrukci     
tInstrukce newInstr(tITyp op, tData *adr1, tData *adr2, tData *vysl);
void ILInit (tInstrList *I);
void ILInsertLast (tInstrList *I, tInstrukce instr);
void ILFirst (tInstrList *I);

//funkce funkci
void GInit (tGList *G);
void GInsertLast (tGList *G, tGData *funkce);
void GFirst (tGList *G);

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