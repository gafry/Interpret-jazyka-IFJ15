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

tErrVyr ResolveToken(tToken token, TokenResolved t)
{
	switch(token.stav)
	{
		case S_ADD:
					t.operace = PLUS;
					break;

		case S_DEK:
					t.operace = DEKREMENT;	//DEKREMENT pridat do tTabOperace
					break;

		case S_DIV:
					t.operace = DELENO;
					break;

		case S_INK:
					t.operace = INKREMENT;	//INKREMENT pridat do tTabOperace
					break;

		case S_MEN:
					t.operace = MENSI;
					break;

		case S_MUL:
					t.operace = KRAT;
					break;

		case S_PRIR:
					t.operace = PRIRAD;	//PRIRAD pridat do tTabOperace
					break;

		case S_ROV:
					t.operace = ROVNASE;
					break;

		case S_NEROV:
					t.operace = NEROVNASE;
					break;

		case S_MENROV:
					t.operace = MENSIROVNO;
					break;

		case S_VECROV:
					t.operace = VETSIROVNO;
					break;

		case S_SUB:
					t.operace = MINUS;
					break;

		case S_VEC:
					t.operace = VETSI;
					break;

		case S_VYKR:
					t.operace = NEGACE;	//NEGACE pridat do tTabOperace
					break;
	}

	//return ERR_OK;
}
