#include "ial.h"
#include "alokator.h"

int TRPVELIKOST = TRP_VELIKOST;

int hash(tKlic klic) {
	int pom = 1;
	int delka = strlen(klic);

	for (int i=0; i<delka; i++){
		pom += klic[i];
	}

	return (pom % TRPVELIKOST);
}

void TRPInit(tTabulka *ptr){
	if (ptr != NULL) {
		for (int i=0; i < TRPVELIKOST; i++){
			(*ptr) [i] = NULL;
		}
	}
	//error
}

tTRPPolozka *TRPSearch(tTabulka *ptr, tKlic klic){
	if (ptr != NULL){
		tTRPPolozka *polozka = (*ptr) [hash(klic)];

		while (polozka != NULL){
			if (!(strcmp(polozka->klic, klic))){
				return polozka;
			} else {
				polozka = polozka->next;
			}
		}
		return NULL;
	} else {
		//error
		return NULL;
	}
}

void TRPInsert(tTabulka *ptr, tKlic klic, tData data){
	if (ptr != NULL){
		tTRPPolozka *polozka = TRPSearch(ptr, klic);
		tTRPPolozka *pom;

		if (polozka != NULL){
			polozka->data = data;
		} else {
			polozka = (tTRPPolozka*) newMalloc(sizeof(tTRPPolozka));
			if (polozka == NULL){
				//error
				return;
			} else {
				polozka->data = data;
				polozka->klic = klic;
				int index = hash(klic);
				pom = (*ptr) [index];
				polozka->next = pom;
				(*ptr)[index] = polozka;
			}
		}
	}
	//error
}

tData *TRPData(tTabulka *ptr, tKlic klic){
	tTRPPolozka polozka = TRPSearch(ptr, klic);
	if (polozka != NULL){
		return &polozka->data;
	} else {
		return NULL;
	}
}

