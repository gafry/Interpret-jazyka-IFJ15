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

#include "ial.h"

int TRPVELIKOST = TRP_VELIKOST;

// hashovaci funkce
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
}

// vrati tTRPPolozku pro dany klic
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

// vlozi prvek do tabulky
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

// vrati tData pro dany klic
tData *TRPData(tTabulka *ptr, tKlic klic){
	tTRPPolozka *polozka = TRPSearch(ptr, klic);
	if (polozka != NULL){
		return polozka->data;
	} else {
		return NULL;
	}
}

// zkopiruje tabulku
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
                } else if (pom->data->typ == 7){
                	dataFrame->typ = 7;
                }
				dataFrame->nazev = pom->data->nazev;
				dataFrame->def = false;
				dataFrame->ramec = pom->data->ramec;

				TRPInsert(tabEmpty, pom->klic, dataFrame);
			}
		}
	}
}

// NULL tabulky
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

void shellSort(char *s1){
	char pom = NULL;
	int x, i, c, next;
	int w;
	x = strlen(s1);
	x = x/2;
	c = x;
	i = 0;

	while(1) {
		w = i;
		while (1) {
			if (w < 0){
				w = w + (x - i);
				pom = s1[w];
				s1[w] = s1[x];
				s1[x] = pom;
				continue;
			}
			if (s1[w] > s1[x]){
			w = w - (x - i);
			} else {
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
				return ;
			}
			c = c/2;
			x = c;
			i = 0;
			continue;
		}
		x++;
		i++;
	}
	return ;
}

int boyer_moore (char *s, char *search){
	int i = 0;
	int delka, delka2;
	int pom; // pomocná proměnná
	int akt; // aktualní pozice v řetezci s

	delka = strlen(search) - 1; // delka retezce search o jednu menší
	delka2 = strlen(s) - 1; // delka retezce s o jednu menší
	pom = delka;
	i = delka;
	akt = delka;

	while(1){
		if (akt > delka2){
			return -1;
		}
		i = delka;
		pom = akt;
		if(s[akt] == search[delka]){
			do{
				if((i-1) < 0){
						return pom;
				}
				if (s[pom-1] == search[i-1]){ // porovnávání znaků 
						i--;
						pom--;
				}
				else{
					i--;
					while(i >= 0){
				 		if (s[pom] == search[i]){ // V řetezci search vyhledáváme schodný znak se znakem na pozici s[pom]
								akt = akt + delka - i; //zmenime aktualni pozici na retezci s
								pom = akt;
								i = akt;
								break;
							}
						else{
							i--;
						}	
					}
					if(i == -1){
						akt = akt + delka - i; //zmenime aktualni pozici na retezci s
					}	
					i = -2;
				}
			}while(i != -2);
		}
		else{
			i--;
			while(i >= 0){
				if (s[akt] == search[i]){ // V řetezci search vyhledáváme schodný znak se znakem na pozici s[akt]
					akt = akt + delka - i; //zmenime aktualni pozici na retezci s
					i = akt;
					break;
				}
				else{
					i--;
				}
			}
			if(i == -1){
			akt = akt + delka - i; //zmenime aktualni pozici na retezci s
			}	
		}

	}
}