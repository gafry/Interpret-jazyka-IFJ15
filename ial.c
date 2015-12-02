#include "ial.h"

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

void TRPInsert(tTabulka *ptr, tKlic klic, tData *data){
	if (ptr != NULL){
		tTRPPolozka *polozka = TRPSearch(ptr, klic);
		tTRPPolozka *pom;

		if (polozka != NULL){
			polozka->data = data;
		} else {
			polozka = (tTRPPolozka*) newMalloc(sizeof(tTRPPolozka));
			if (polozka == NULL){
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
}

tData *TRPData(tTabulka *ptr, tKlic klic){
	tTRPPolozka *polozka = TRPSearch(ptr, klic);
	if (polozka != NULL){
		return polozka->data;
	} else {
		return NULL;
	}
}

void TRPVynulluj(tTabulka *ptr){
	if (ptr != NULL) { 
		for (int i = 0; i < TRPVELIKOST; i++) {
			tTRPPolozka *polozka = (*ptr)[i];
			tTRPPolozka *pom;
			while (polozka != NULL) {
                pom = polozka;
                polozka = polozka->next;
				free(pom);
			}
            (*ptr)[i] = NULL;
		}
	}
}

void TRPCopy(tTabulka *tabFull, tTabulka *tabEmpty){
	if (tabFull != NULL) { 
		for (int i = 0; i < TRPVELIKOST; i++) {
			tTRPPolozka *polozka = (*tabFull)[i];
			tTRPPolozka *pom;
			while (polozka != NULL) {
                pom = polozka;
                polozka = polozka->next;				

                tData *dataFrame = newMalloc(sizeof(tData));

                dataFrame->hodnota = newMalloc(sizeof(tHodnota));

                if (pom->data->typ == 1){
					dataFrame->typ = 1;
                } else if (pom->data->typ == 2){
					dataFrame->typ = 2;
                } else if (pom->data->typ == 3){
					dataFrame->typ = 3;
					dataFrame->hodnota->s = NULL;
                }
				dataFrame->nazev = pom->data->nazev;
				dataFrame->def = false;
				dataFrame->boss = pom->data->boss;

				TRPInsert(tabEmpty, pom->klic, dataFrame);
			}
		}
	} //tTRPPolozka *x = TRPSearch(tabEmpty, "a"); printf("%s\n", x->data->nazev);
}