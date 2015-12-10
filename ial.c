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

/*void TRPVynulluj(tTabulka *ptr){
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
}*/

void TRPCopy(tTabulka *tabFull, tTabulka *tabEmpty){
	if (tabFull != NULL) { 
		for (int i = 0; i < TRPVELIKOST; i++) {
			tTRPPolozka *polozka = (*tabFull)[i];
			tTRPPolozka *pom;
			while (polozka != NULL) {
                pom = polozka;
                polozka = polozka->next;				

                tData *dataFrame = newMalloc(sizeof(tData));
                if (error != ERR_OK) return;

                dataFrame->hodnota = newMalloc(sizeof(tHodnota));
                if (error != ERR_OK) return;

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
				dataFrame->ramec = pom->data->ramec;

				TRPInsert(tabEmpty, pom->klic, dataFrame);
			}
		}
	} //tTRPPolozka *x = TRPSearch(tabEmpty, "a"); printf("%s\n", x->data->nazev);
}

void TRPDelete(tTabulka *tab){
	if (tab != NULL) { 
		for (int i = 0; i < TRPVELIKOST; i++) {
			tTRPPolozka *polozka = (*tab)[i];
			tTRPPolozka *pom;
			while (polozka != NULL) {
                pom = polozka;
                polozka = polozka->next;				

                tData *ptr1 = pom->data;
                tHodnota *ptr2 = pom->data->hodnota;

                free(ptr2);
                free(ptr1);
			}
			(*tab)[i] = NULL;
		}
	}
}

/*char *shellSort(char *s1){
	char pom;
	int x, i, w, c, next;
	x = strlen(s1);
	x = x/2;
	c = x;
	i = 0;

	while(1) {
		w = i;
		while (1){
			if (w < 0){
				w = w + (x - i);
				pom = s1[w];
				s1[w] = s1[x];
				s1[x] = pom;
				continue;
			}
			if (s1[w] > s1[x]){
				w = w - (x - i);
			}
			else {
				w = w + (x - i);
				if ((x - w) == 0) break;
				else if (s1[w] > s1[x]){
					pom = s1[x];
					next = x;
					while(next > w){
						s1[next] = s1[next-1];
						next = next - 1;
					}
				}
				s1[w] = pom;
				break;
			}
		}

	if (s1[x+1] == '\0'){
		if((x-i) == 1){
			return s1;
			c = c/2;
			x = c;
			i = 0;
			continue;
		}
		x++;
		i++;
	}
}*/