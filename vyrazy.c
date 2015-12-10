#include "vyrazy.h"

tZasobnik PAZasobnik;
tToken token;
tInstrukce aktInstrukce;
tError error;
tPrvekPtr pomZasobnik;
tSADataPtr dataZasobnik;
tSADataPtr dataToken;

char *pomNazev;
tInstrList pomPaska;

const tTab precedencni_tabulka[RADKY][SLOUPCE]=
{
		//   +  -  *  /  == != <= >= <  >  (  )  ID $
	/*+*/	{V, V, M, M, V, V, V, V, V, V, M, V, M, V},
	/*-*/	{V, V, M, M, V, V, V, V, V, V, M, V, M, V},
	/***/	{V, V, V, V, V, V, V, V, V, V, M, V, M, V},
	/*/*/	{V, V, V, V, V, V, V, V, V, V, M, V, M, V},
	/*==*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*!=*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*<=*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*>=*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*<*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*>*/	{M, M, M, M, V, V, V, V, V, V, M, V, M, V},
	/*(*/	{M, M, M, M, M, M, M, M, M, M, M, R, M, C},
	/*)*/	{V, V, V, V, V, V, V, V, V, V, C, V, C, V},
	/*ID*/	{V, V, V, V, V, V, V, V, V, V, C, V, C, V},
	/*$*/	{M, M, M, M, M, M, M, M, M, M, M, C, M, C},
};

tError expr(char **last){

	int i = 0;	

	VInit(&PAZasobnik);

	dataZasobnik = newMalloc(sizeof(struct tSAData));
	if (dataZasobnik == NULL) return ERR_INTER;

	dataZasobnik->typ = DOLAR;
	VPush(&PAZasobnik, dataZasobnik);
	dataZasobnik = NULL;

	while(1){

		dataToken = ResolveToken(token, i);
		if (error != ERR_OK) return error;
		i++;

		dataZasobnik = findNonE (&PAZasobnik);

		//printf("zasobnik : %s ... %d, token : %s ... %d\n", dataZasobnik->nazev, dataZasobnik->typ, dataToken->nazev, dataToken->typ);

		switch(precedencni_tabulka[dataZasobnik->typ][dataToken->typ]){

			case V:

				if (dataZasobnik->typ == ID && dataZasobnik->znamenko == 1){ 
					dataZasobnik->znamenko = 0;
					dataZasobnik->E = true;
				} else {
					makeInstr (&PAZasobnik);
				}
				break;

			case M:

				if (dataToken->typ == ID){  
					dataToken->znamenko = 1;
					VPush(&PAZasobnik, dataToken);					
				} else if (dataZasobnik->typ == DOLAR){ 
					pomZasobnik = findSth (&PAZasobnik, DOLAR);
					if (pomZasobnik->lptr != NULL){
						if (pomZasobnik->lptr->data->E == true){
							pomZasobnik->lptr->data->znamenko = 1;
						} else {
							if (pomZasobnik->lptr->data->typ != LKULZAV){
								return ERR_SYN;
							}
						}
					} else {
						if (dataToken->typ != LKULZAV) return ERR_SYN;
					}
					VPush(&PAZasobnik, dataToken);
				} else if (dataZasobnik->typ == LKULZAV && dataZasobnik->typ == LKULZAV){ 
					pomZasobnik = findSth (&PAZasobnik, dataZasobnik->typ);
					if (pomZasobnik->lptr != NULL){
						if (pomZasobnik->lptr->data->E){
							pomZasobnik->lptr->data->znamenko = 1;
						} else if (pomZasobnik->rptr->data->E){
							pomZasobnik->rptr->data->znamenko = 1;
						}
					} else if (pomZasobnik->rptr->data->E){
						pomZasobnik->rptr->data->znamenko = 1;
					}
					VPush(&PAZasobnik, dataToken);
				} else {  
					pomZasobnik = findSth (&PAZasobnik, dataZasobnik->typ);
					if (pomZasobnik->lptr != NULL){
						if (pomZasobnik->lptr->data->E){
							pomZasobnik->lptr->data->znamenko = 1;
						} else if (pomZasobnik->rptr->data->E){
							pomZasobnik->rptr->data->znamenko = 1;
						} else return ERR_SYN;
					} else if (pomZasobnik->rptr->data->E){
						pomZasobnik->rptr->data->znamenko = 1;
					} else return ERR_SYN;
					VPush(&PAZasobnik, dataToken);
				}
				token = getToken();
				if (error != ERR_OK) return error;
				break;

			case R:

				VPopZavorka(&PAZasobnik);

				token = getToken();
				if (error != ERR_OK) return error;

				break;

			case C:

				if (dataZasobnik->typ == DOLAR && dataToken->typ == DOLAR){
					pomZasobnik = findSth (&PAZasobnik, DOLAR);
					if (pomZasobnik->lptr->data->E){
						*last = pomZasobnik->lptr->data->nazev;
						VClear(&PAZasobnik);
						return ERR_OK;
					} else return ERR_SYN;
				} else return ERR_SYN;
				break;
		}
	}
}

tSADataPtr ResolveToken (tToken token, int i){

	if (i == 0)
	{
		switch(token.stav) /// když prvni znak neni int, double, string, nebo id
		{
			case S_ADD:
			case S_PZ:
			case S_DIV:
			case S_MEN:
			case S_MUL:
			case S_ROV:
			case S_NEROV:
			case S_MENROV:
			case S_VECROV:
			case S_SUB:
			case S_VEC:
			case S_STREDNIK:
				error = ERR_SYN;
				return NULL;
				break;

			default: 
				break;
		}
	}

	dataZasobnik = newMalloc(sizeof(struct tSAData));
	if (dataZasobnik == NULL){
		error = ERR_SYN;
		return NULL;
	}

	switch(token.stav)
	{
		case S_ADD:
				dataZasobnik->typ = PLUS;	// +
				break;
		
		case S_DIV:
				dataZasobnik->typ = DELENO;	// /
				break;
		
		case S_MEN:
				dataZasobnik->typ = MENSI;	// <
				break;

		case S_MUL:
				dataZasobnik->typ = KRAT;	// *
				break;

		case S_ROV:
				dataZasobnik->typ = ROVNASE;	// ==
				break;

		case S_NEROV:
				dataZasobnik->typ = NEROVNASE;	// !=
				break;

		case S_MENROV:printf("%d\n", token.stav);
				dataZasobnik->typ = MENSIROVNO;	// <=
				break;

		case S_VECROV:
				dataZasobnik->typ = VETSIROVNO;	// >=
				break;

		case S_SUB:
				dataZasobnik->typ = MINUS;	// -
				break;

		case S_VEC:
				dataZasobnik->typ = VETSI;	// >
				break;
		
		case S_SLZ:
		case S_STREDNIK:
				dataZasobnik->typ = DOLAR;	//terminal
				break;

		case S_LZ:
				dataZasobnik->typ = LKULZAV;	// (
				break;

		case S_PZ:
				dataZasobnik->typ = PKULZAV;	// )
				break;

		case S_TYPDOUBLE:	//hodnota double
				pomNazev = newDouble(halda, token.data);
				dataZasobnik->znamenko = 0;
				dataZasobnik->nazev = pomNazev;
				dataZasobnik->typ = ID;
				dataZasobnik->E = false;
				break;

		case S_TYPINT:	//hodnota int
				pomNazev = newInt(halda, token.data);
				dataZasobnik->znamenko = 0;
				dataZasobnik->nazev = pomNazev;
				dataZasobnik->typ = ID;
				dataZasobnik->E = false;
				break;

		case S_TYPSTRING:	//retezec
				pomNazev = newStr(halda, token.data);
				dataZasobnik->znamenko = 0;
				dataZasobnik->nazev = pomNazev;
				dataZasobnik->typ = ID;
				dataZasobnik->E = false;
				break;

		case S_IDENT:	
				dataZasobnik->znamenko = 0;
				dataZasobnik->nazev = token.data;
				dataZasobnik->typ = ID;
				dataZasobnik->E = false;
				break;

		default:
				error = ERR_SYN;	
				return NULL;
				break;
	}

	return dataZasobnik;
}

tSADataPtr findNonE (tZasobnik *zasobnik){

	tPrvekPtr pom = zasobnik->vrchol;
	while (1){

		if (pom->data->E != true){
			return pom->data;
		}

		pom = pom->rptr;
	}
}

tPrvekPtr findSth (tZasobnik *zasobnik, tTabOperace typ){

	tPrvekPtr pom = zasobnik->vrchol;
	while (1){
		if (pom->data->typ == typ){
			return pom;
		}

		pom = pom->rptr;
	}
}

void makeInstr (tZasobnik *zasobnik){

	char *name1;
	char *name2;
	char *name3;
	tTabOperace operace;
	tPrvekPtr pom = zasobnik->vrchol;
	if ((pom->data->E == false) || pom->data->znamenko == 1){ 
		error = ERR_SYN;
		return ;
	}
	name1 = pom->data->nazev;
	pom = pom->rptr;
	if (pom->data->typ == DOLAR || pom->data->typ == ID || pom->data->typ == LKULZAV || pom->data->typ == PKULZAV){
		error = ERR_SYN;
		return ;
	}
	operace = pom->data->typ;
	pom = pom->rptr;
	if ((pom->data->E == false) || pom->data->znamenko == 0){
		error = ERR_SYN;
		return ;
	}
	name2 = pom->data->nazev;
	name3 = newUnkown(halda);

	if (operace == PLUS){
		aktInstrukce = newInstr(I_ADD, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == MINUS){
		aktInstrukce = newInstr(I_SUB, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == KRAT){
		aktInstrukce = newInstr(I_MUL, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == DELENO){
		aktInstrukce = newInstr(I_DIV, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == VETSI){
		aktInstrukce = newInstr(I_VET, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == MENSI){
		aktInstrukce = newInstr(I_MEN, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == VETSIROVNO){
		aktInstrukce = newInstr(I_VETROV, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == MENSIROVNO){
		aktInstrukce = newInstr(I_MENROV, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == ROVNASE){
		aktInstrukce = newInstr(I_ROV, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else if (operace == NEROVNASE){
		aktInstrukce = newInstr(I_NEROV, name2, name1, name3);
		ILInsertLast(&pomPaska, aktInstrukce);
	} else {
		error = ERR_SYN;
		return ;
	}

	dataZasobnik = newMalloc(sizeof(struct tSAData));
	if (dataZasobnik == NULL) return ;

	dataZasobnik->znamenko = 0;
	dataZasobnik->nazev = name3;
	dataZasobnik->typ = ID;
	dataZasobnik->E = true;

	VPop(zasobnik);
	VPop(zasobnik);
	VPop(zasobnik);
	VPush(zasobnik, dataZasobnik);
}

void VInit(tZasobnik *zasobnik) {
	zasobnik->vrchol = NULL;
	zasobnik->Act = NULL;
}

int VEmpty(tZasobnik *zasobnik) {
	return (zasobnik->vrchol == NULL) ? 1 : 0;
}

void VPush(tZasobnik *zasobnik, tSADataPtr data) {

	tPrvekPtr newItem;
	newItem = newMalloc(sizeof(struct tPrvek));
	if (newItem == NULL) return ;

	if (zasobnik->vrchol == NULL){
		newItem->rptr = NULL;
	} else {
		newItem->rptr = zasobnik->vrchol;
		zasobnik->vrchol->lptr = newItem;
	}
	
	newItem->lptr = NULL;
	newItem->data = data;
	zasobnik->vrchol = newItem;
}

void VPop(tZasobnik *zasobnik) {
	if (!(VEmpty(zasobnik))) {
		tPrvekPtr item = zasobnik->vrchol;
		zasobnik->vrchol = item->rptr;
		item = NULL;
	}
}

void VPopZavorka(tZasobnik *zasobnik) {
	if (!(VEmpty(zasobnik))) {
		tPrvekPtr item = zasobnik->vrchol;
		item = item->rptr;
		zasobnik->vrchol->rptr = item->rptr;
		item->rptr->lptr = zasobnik->vrchol;
		item = NULL;
	}
}

void VClear(tZasobnik *zasobnik) {
	while (zasobnik->vrchol != NULL) {
		VPop(zasobnik);
	}
}

char *newInt(tTabulka* halda, char *hodnota){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	int hodnota2 = atoi(hodnota);
	dataFrame->hodnota = newMalloc(sizeof(tHodnota));
	dataFrame->hodnota->i = hodnota2;
	dataFrame->typ = 1;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}

char *newDouble(tTabulka* halda, char *hodnota){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	double hodnota2;
	sscanf(hodnota, "%lf", &hodnota2);
	dataFrame->hodnota = newMalloc(sizeof(tHodnota));
	dataFrame->hodnota->d = hodnota2;
	dataFrame->typ = 2;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}

char *newStr(tTabulka* halda, char *hodnota){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->hodnota = newMalloc((strlen(key)+1)*sizeof(char));
	dataFrame->hodnota->s = hodnota;
	dataFrame->typ = 3;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}

char *newUnkown(tTabulka* halda){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->typ = 0;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}