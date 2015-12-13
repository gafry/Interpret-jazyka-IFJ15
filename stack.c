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

#include "stack.h"

tTabulka *halda;

void ZInit(tStack *zasobnik) {

	zasobnik->vrchol = NULL;
}

int ZEmpty(tStack *zasobnik) {

	return (zasobnik->vrchol == NULL) ? 1 : 0;
}

// pushne na vrchol
void ZPush(tStack *zasobnik, tTabulka *tab, int ramec) {

	struct tStackPrvek *newItem;
	newItem = newMalloc(sizeof(struct tStackPrvek));
	if (newItem == NULL){
		return ;
	}

	newItem->tab = (tTabulka *) newMalloc(sizeof(tTabulka));
	if (error != ERR_OK){
		return ;
	}

	TRPInit(newItem->tab);
	TRPCopy(tab, newItem->tab);

	newItem->next = zasobnik->vrchol;
	newItem->ramec = ramec;
	zasobnik->vrchol = newItem;
}

// popne vrchol
void ZPop(tStack *zasobnik, int i) {

	if (!(ZEmpty(zasobnik))) {
		tStackPrvekPtr item = zasobnik->vrchol;
		//tTabulka *ptr1;
		bool pomBool = true;
		while (pomBool){ 
			if (item->ramec == i){
				pomBool = false;
			}
			item = item->next;
			/*ptr1 = item->tab;
			TRPDelete(ptr1);
			free(item);*/
		}
		zasobnik->vrchol = item;
	}
}

//vyhleda promennou
tTRPPolozka *ZSearch(tStack *zasobnik, char *nazev) {

	tTRPPolozka *pom;
	tStackPrvekPtr pom1 = zasobnik->vrchol;
	bool x = true;

	while (x){
		pom = TRPSearch(pom1->tab, nazev);

		if (pom == NULL){
			if (pom1->ramec != 1){
				pom1 = pom1->next;
			} else x = false;
		} else if (strcmp(nazev, pom->data->nazev) == 0){
			x = false;
			return pom;
		} else {
			if (pom1->ramec != 1){
				pom1 = pom1->next;
			} else x = false;
		}
	}

	pom = TRPSearch(halda, nazev);
	return pom;
}

// vyhleda promennou i pod ramcem 1
tTRPPolozka *ZRekurzeSearch(tStack *zasobnik, char *nazev) {

	tTRPPolozka *pom;
	tStackPrvekPtr pom1 = zasobnik->vrchol;
	bool x = true;
	bool y = true;

	while (y){ 
		if (pom1->ramec == 1){
			y = false;
			pom1 = pom1->next;
		} else pom1 = pom1->next;
	}

	while (x){
		pom = TRPSearch(pom1->tab, nazev);

		if (pom == NULL){ 
			if (pom1->ramec != 1){
				pom1 = pom1->next;
			} else x = false;
		} else if (strcmp(nazev, pom->data->nazev) == 0){
			x = false;
			return pom;
		} else {
			if (pom1->ramec != 1){
				pom1 = pom1->next;
			} else x = false;
		}
	}

	pom = TRPSearch(halda, nazev);
	return pom;
}