#include "vyrazy.h"
#include "alokator.h"

void ZInit(tZasobnik *zasobnik) {
	zasobnik->vrchol = NULL;
}

int ZEmpty(tZasobnik *zasobnik) {
	return (zasobnik->vrchol == NULL) ? 1 : 0;
}

void ZPush(tZasobnik *zasobnik, int data) {

	struct tPrvek *newItem;
	newItem = newMalloc(sizeof(struct tPrvek));
	if (newItem == NULL){
		//error
		return ;
	}

	newItem->next = zasobnik->vrchol;
	newItem->data = data;
	zasobnik->vrchol = newItem;
}

void ZPop(tZasobnik *zasobnik) {
	if (!(ZEmpty(zasobnik))) {
		tPrvekPtr item = zasobnik->vrchol;
		zasobnik->vrchol = item->next;
		free(item);
	}
}

void ZTop(tZasobnik *zasobnik, int *data) {
	if (ZEmpty(zasobnik)) {
		return NULL;
	} else {
		*data = zasobnik->vrchol->data;
	}
}

void ZTopPop(tZasobnik *zasobnik, int *data) {
	ZTop(zasobnik, data);
	ZPop(zasobnik);
}

void ZClear(tZasobnik *zasobnik) {
	while (zasobnik->vrchol != NULL) {
		ZPop(zasobnik);
	}
}