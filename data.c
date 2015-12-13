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
	I->Pom = NULL;
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
	newItem->jump = NULL;

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

bool GFind (tGList *G, char *name){
	GFirst(G);
	while (1){
		if (G->Act == NULL){
			return false;
		}

		if (!(strcmp(G->Act->funkce->nazev, name))){
			return true;
		} else G->Act = G->Act->rptr;
	}
}

tGData *GFindData (tGList *G, char *name){
	GFirst(G);
	while (1){
		if (G->Act == NULL){
			return NULL;
		}

		if (!(strcmp(G->Act->funkce->nazev, name))){
			return G->Act->funkce;
		} else G->Act = G->Act->rptr;
	}
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

//parametry ------------------------------------------------------------------------------------------------

void PInit (tParamList *P){
	P->First = NULL;
	P->Act = NULL;
	P->Last = NULL;
}

bool PCheck (tParamElemPtr par1, tParamElemPtr par2){

	while(1){
		if (par1 == NULL && par2 == NULL){ 
			return true;
		} else if (par1 == NULL || par2 == NULL){
			return false;
		} else { 
			if ((par1->data->typ != par2->data->typ) || strcmp(par1->data->nazev, par2->data->nazev)){
				return false;
			} else {
				par1 = par1->rptr;
				par2 = par2->rptr;
			}
		}
	}
}

void PInsertTab (tParamElemPtr par, tTabulka *tab){

	while(1){
		if (par == NULL){
			return;
		}

		TRPInsert(tab, par->data->nazev, par->data);
		par = par->rptr;
	}
}

void PInsertLast (tParamList *P, int typ, char *nazev){

	struct tParamElem *newItem;
	newItem = newMalloc(sizeof(struct tParamElem));
	if (error != ERR_OK){
		return;
	}

	tData *dataFrame = newMalloc(sizeof(tData));
	if (error != ERR_OK) return;

    dataFrame->typ = typ;
    dataFrame->nazev = nazev;
    dataFrame->def = false;
    dataFrame->ramec = 0;

	newItem->data = dataFrame;
	newItem->lptr = NULL;
	newItem->rptr = NULL;

	if (P->First == NULL) {
		P->First = newItem;
	}

	newItem->lptr = P->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	P->Last = newItem;
}

void PInsertArg (tParamList *P, int typ, char *nazev, int i, double d, char *s){

	struct tParamElem *newItem;
	newItem = newMalloc(sizeof(struct tParamElem));
	if (error != ERR_OK){
		return;
	}

	tData *dataFrame = newMalloc(sizeof(tData));
	if (error != ERR_OK) return;

	dataFrame->hodnota = newMalloc(sizeof(tHodnota));
    if (error != ERR_OK) return;

    dataFrame->nazev = NULL;

    if (typ == 0){
    	dataFrame->nazev = nazev;
    } else if (typ == 1){
    	dataFrame->hodnota->i = i;
    } else if (typ == 2){
    	dataFrame->hodnota->d = d;
    } else if (typ == 3){
    	dataFrame->hodnota->s = s;
    }

    dataFrame->typ = typ;    
    dataFrame->def = false;
    dataFrame->ramec = 0;

	newItem->data = dataFrame;
	newItem->lptr = NULL;
	newItem->rptr = NULL;

	if (P->First == NULL) {
		P->First = newItem;
	}

	newItem->lptr = P->Last;

	if (newItem->lptr != NULL) {
		newItem->lptr->rptr = newItem;
	}

	P->Last = newItem;
}