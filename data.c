#include "data.h"

//instrukce -------------------------------------------------------------------------------------------------

tInstrukce newInstr(tITyp op, char *adr1, char *adr2, char *vysl){
	tInstrukce I;
	I.op = op;
	I.adr1 = adr1;
	I.adr2 = adr2;
	I.vysl = vysl;
	return I;
}

void ILInit (tInstrList *I){
	I->First = NULL;
	I->Act = NULL;
	I->Last = NULL;
}

void ILInsertLast (tInstrList *I, tInstrukce instr){

	struct tInstrElem *newItem;
	newItem = newMalloc(sizeof(struct tInstrElem));
	if (error != ERR_OK){
		return;
	}

	newItem->instr = instr;
	newItem->rptr = NULL;
	newItem->lptr = NULL;

	if (I->First == NULL) {
		I->First = newItem;
	}

	newItem->lptr = I->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	I->Last = newItem;
}

void ILFirst (tInstrList *I){
	I->Act = I->First;
}

//funkce ---------------------------------------------------------------------------------------------------

void GInit (tGList *G){
	G->First = NULL;
	G->Act = NULL;
	G->Last = NULL;
}

void GInsertLast (tGList *G, tGData *funkce){

	struct tGElem *newItem;
	newItem = newMalloc(sizeof(struct tGElem));
	if (error != ERR_OK){
		return;
	}

	newItem->funkce = funkce;
	newItem->rptr = NULL;
	newItem->lptr = NULL;

	if (G->First == NULL) {
		G->First = newItem;
	}

	newItem->lptr = G->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	G->Last = newItem;
}

void GFirst (tGList *G){
	G->Act = G->First;
}

void GCopyMain (tGList *G, tInstrList *I){

	GFirst(G);
	tGElemPtr pom = G->First;

	while (strcmp(pom->funkce->nazev, "main")){
		pom = pom->rptr;
	}

	tInstrElemPtr pom2 = pom->funkce->paskaZ;
	tInstrElemPtr pom3 = pom->funkce->paskaK;

	while (pom2 != pom3){

		ILInsertLast (I, pom2->instr);
		pom2 = pom2->rptr;
	}

	ILInsertLast (I, pom2->instr);
}

//tabulky -------------------------------------------------------------------------------------------------

void TSInit (tTSList *TS){
	TS->First = NULL;
	TS->Act = NULL;
	TS->Last =NULL;
}

void TSInsertLast (tTSList *TS, tTabulka *tab){

	struct tTSElem *newItem;
	newItem = newMalloc(sizeof(struct tTSElem));
	if (error != ERR_OK){
		return;
	}

	newItem->ts = tab;
	newItem->lptr = NULL;
	newItem->rptr = NULL;

	if (TS->First == NULL) {
		TS->First = newItem;
	}

	newItem->lptr = TS->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	TS->Last = newItem;
}

/*void DLInitList (tDLList *L) {

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {

	struct tDLElem *newItem;
	newItem = newMalloc(sizeof(struct tDLElem));
	if (newItem == NULL) {
		return;
	}

	newItem->data = val;
	newItem->rptr = NULL;
	newItem->lptr = NULL;
	
	if (L->First == NULL) {
		L->Last = newItem;
	}

	newItem->rptr = L->First;

	if (newItem->rptr != NULL) {
		newItem->rptr->lptr = newItem;
	}

	L->First = newItem;
}

void DLInsertLast(tDLList *L, int val) {

	struct tDLElem *newItem;
	newItem = newMalloc(sizeof(struct tDLElem));
	if (newItem == NULL) {
		return;
	}

	newItem->data = val;
	newItem->rptr = NULL;
	newItem->lptr = NULL;

	if (L->First == NULL) {
		L->First = newItem;
	}

	newItem->lptr = L->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	L->Last = newItem;
	
}

void DLFirst (tDLList *L) {

	L->Act = L->First;

}

void DLLast (tDLList *L) {
	
	L->Act = L->Last;
	
}

void DLCopyFirst (tDLList *L, int *val) {

	if (L->First == NULL) {
		return;
	} else {
		*val = L->First->data;
	}
}

void DLCopyLast (tDLList *L, int *val) {
	
	if (L->First == NULL) {
		return;
	} else {
		*val = L->Last->data;
	}
}

void DLPostInsert (tDLList *L, int val) {
	
	if (L->Act != NULL) {
        
    	struct tDLElem *newItem;
    	if ((newItem = newMalloc(sizeof(struct tDLElem))) == NULL) {
        	return;
	    }

    	newItem->data = val;
    	newItem->rptr = NULL;
    	newItem->lptr = NULL;
 
    	if (L->Act->rptr != NULL) {
        	L->Act->rptr->lptr = newItem;
        	newItem->rptr = L->Act->rptr;
    	} else
    	    L->Last = newItem;
 
    	L->Act->rptr = newItem;
    	newItem->lptr = L->Act;

	}
}

void DLPreInsert (tDLList *L, int val) {
	
	if (L->Act != NULL) { 

    	struct tDLElem *newItem;
    	if ((newItem = newMalloc(sizeof(struct tDLElem))) == NULL) {
    	    return;
    	}

    	newItem->data = val;
    	newItem->rptr = NULL;
    	newItem->lptr = NULL;
 
    	if (L->Act->lptr != NULL) {
    	    L->Act->lptr->rptr = newItem;
    	    newItem->lptr = L->Act->lptr;
    	} else
    	    L->First = newItem;
 
    	L->Act->lptr = newItem;
    	newItem->rptr = L->Act;

	}
}

void DLSucc (tDLList *L) {
	
	if (L->Act != NULL) {
		L->Act = L->Act->rptr;
	}
}


void DLPred (tDLList *L) {
	
	if (L->Act != NULL) {
		L->Act = L->Act->lptr;
	}
}

int DLActive (tDLList *L) {
	
	return (L->Act == NULL) ? 0 : 1;
	
}*/