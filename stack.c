#include "stack.h"

tTabulka *halda;

void ZInit(tZasobnik *zasobnik) {

	zasobnik->vrchol = NULL;
}

int ZEmpty(tZasobnik *zasobnik) {

	return (zasobnik->vrchol == NULL) ? 1 : 0;
}

void ZPush(tZasobnik *zasobnik, tTabulka *tab, bool boss) {

	struct tPrvek *newItem;
	newItem = newMalloc(sizeof(struct tPrvek));
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
	newItem->boss = boss;
	zasobnik->vrchol = newItem;
}

void ZPop(tZasobnik *zasobnik) {

	if (!(ZEmpty(zasobnik))) {
		tPrvekPtr item = zasobnik->vrchol;
		zasobnik->vrchol = item->next;
		free(item);
	}
}

void ZTop(tZasobnik *zasobnik, tTabulka *tab) {

	if (ZEmpty(zasobnik)) {
		return ;
	} else {
		tab = zasobnik->vrchol->tab;
	}
}

void ZTopPop(tZasobnik *zasobnik, tTabulka *tab) {

	ZTop(zasobnik, tab);
	ZPop(zasobnik);
}

void ZClear(tZasobnik *zasobnik) {

	while (zasobnik->vrchol != NULL) {
		ZPop(zasobnik);
	}
}

tTRPPolozka *ZSearch(tZasobnik *zasobnik, char *nazev) {

	tTRPPolozka *pom;
	tPrvekPtr pom1 = zasobnik->vrchol;
	bool x = true;

	while (x){
		pom = TRPSearch(pom1->tab, nazev);

		if (strcmp(nazev, pom->data->nazev) == 0){
			x = false;
			return pom;
		} else {
			if (pom1->boss == false){
				pom1 = pom1->next;
			} else x = false;
		}
	}

	pom = TRPSearch(halda, nazev);
	return pom;
}