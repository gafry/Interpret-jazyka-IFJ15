#include "vyrazy.h"
#include "alokator.h"
#include "precedencni_tabulka.h"
#include "lexikalni_analyzator.h"

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

tErrVyr ResolveToken(tToken token, tTokenResolved *t)
{
	switch(token.stav)
	{
		case S_ADD:
				t.operace = PLUS;	// +
				break;

		case S_DEK:
				t.operace = DEKREMENT;	// --
				break;

		case S_DIV:
				t.operace = DELENO;	// /
				break;

		case S_INK:
				t.operace = INKREMENT;	// ++
				break;

		case S_MEN:
				t.operace = MENSI;	// <
				break;

		case S_MUL:
				t.operace = KRAT;	// *
				break;

		case S_PRIR:
				t.operace = PRIRAD;	// =
				break;

		case S_ROV:
				t.operace = ROVNASE;	// ==
				break;

		case S_NEROV:
				t.operace = NEROVNASE;	// !=
				break;

		case S_MENROV:
				t.operace = MENSIROVNO;	// <=
				break;

		case S_VECROV:
				t.operace = VETSIROVNO;	// >=
				break;

		case S_SUB:
				t.operace = MINUS;	// -
				break;

		case S_VEC:
				t.operace = VETSI;	// >
				break;

		case S_VYKR:
				t.operace = NEGACE;	// !
				break;

		case S_CARKA:
				t.operace = CARKA;	// ,	//TODO: terminal misto znaku , ?
				break;

		case S_KOMENT:
				t.operace = KOMENT;	// //
				break;

		case S_LKOM:
				t.operace = LKOM;	// /*
				break;

		case S_PKOM:
				t.operace = PKOM;	// */
				break;

		case S_LPRIR:
				t.operace = LPRIR;	// <<
				break;

		case S_PPRIR:
				t.operace = PPRIR;	// >>
				break;

		case S_STREDNIK:
				t.operace = DOLAR;	//terminal
				break;

		case S_LZ:
				t.operace = LKULZAV;	// (
				break;

		case S_PZ:
				t.operace = PKULZAV;	// )

		case S_SLZ:
				t.operace = LMNOZZAV;	// {
				break;

		case S_SPZ:
				t.operace = PMNOZZAV;	// }
				break;

		case S_TYPDOUBLE:	//hodnota double
				t->operace   	  = ID;	
				t->data.promenna  = tDouble;
				t->data.hodnota.d = strtod(token.data, NULL);
				//t->data.nazev = ;	//TODO: vygenerovat nazev do tabulky symbolu

				break;

		case S_TYPINT:	//hodnota int
				t->operace   	  = ID;
				t->data.promenna  = tInt;
				t->data.hodnota.i = strtol(token.data, NULL, 10);
				//t->data.nazev = ;	//TODO: vygenerovat nazev to tabulky symbolu
				break;

		case S_TYPSTRING:	//retezec
				t->operace = ID;
				t->data.promenna = tString;
				t->data.hodnota->s = token.data;	//TODO: kontrola na NULL? (if(token.data == NULL))
				//t->data.nazev = ;	//TODO: vygenerovat nazev do tabulky symbolu
				break;
		/*
		case S_KLIC:
				if(strcmp(token.data, "double") == 0)
				{
					t->operace = ID;
					t->data.promenna = tDouble;
				}
				//TODO: vygenerovat nazev do tabulky symbolu?

				if(strcmp(token.data, "int") == 0)
				{
					t->operace = ID;
					t->data.promenna = tInt;
				}
				//TODO: vygenerovat nazev do tabulky symbolu?

				if(strcmp(token.data, "string") == 0)	//TODO: misto string jiny retezec?
				{
					t->operace = ID;
					t->data.promenna = tString;
				}
				//TODO: vygenerovat nazev do tabulky symbolu?

				if(strcmp(token.data, "") == 0)
				break;
		*/

		case S_CIN:
				t->operace = ID;
				//TODO: konzultovat postup
				break;	

		case S_IDENT:	//identifikator 
						//TODO: doplnit

				break;

		default:
				fprintf(stderr, "Token obsahuje neznamy symbol!!\n");
				return S_CHYBAVS;	
				break;
	}

	//return ERR_OK;
}
