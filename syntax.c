#include "syntax.h"

int main_cnt = 0;
int ramec_check = 0;
char *nazevFunkce;
char *pomFun;
char *randomNull = NULL;
tError error;

tParamList pomParametry;
extern tInstrList pomPaska; //pomocna paska, ktera se pak ulozi do dat aktualni funkce
tGList global; //seznam globalnich funkci
tTSList tabulkaSymbolu; //seznam globalnich promennych

tTabulka *aktTab; //aktualni tabulka
tTabulka *halda; //halda pomocnych promennych
tInstrukce aktInstrukce; //instrukce, ktera se zrovna uklada

tError s_syntax(){

	if (error != ERR_OK){
		return error;
	}

	tToken token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav == S_EOF){

		return ERR_SYN;

	} else {

		GInit(&global);
		TSInit(&tabulkaSymbolu);
		ILInit(&pomPaska);

		halda = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
		TRPInit(halda);

		error = s_funkce();
		if (error != ERR_OK){
			return error;
		}
	}

	if (main_cnt == 0) {
		error = ERR_SEM_NEDEF;
	} else if (main_cnt > 1) {
		error = ERR_SYN;
	}

	return error;
}

tError s_funkce(){

	int typFunkce;

	PInit(&pomParametry);

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_DOUBLE) {
		typFunkce = 2;
	} else if (token.stav == S_INT) {
		typFunkce = 1;
	} else if (token.stav == S_STRING) {
		typFunkce = 3;
	} else if (token.stav == EOF) {
		return error;
	} else return ERR_SYN;
	
	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav != S_IDENT){
		return ERR_SYN;
	}

	if (strcmp("main", token.data)){  //neni main

		if (!strcmp("length", token.data) ||
			!strcmp("substr", token.data) ||
			!strcmp("concat", token.data) ||
			!strcmp("find", token.data) ||
			!strcmp("sort", token.data)){
			return ERR_SEM_NEDEF;
		}

		nazevFunkce = token.data;
		bool insert = false;

		tGData *newItem = GFindData (&global, nazevFunkce);
		if (newItem == NULL){
			insert = true;
			newItem = newMalloc(sizeof(struct tGlobalData));
		} else {
			if (newItem->def){
				return ERR_SEM_NEDEF;
			} else if (newItem->typ != typFunkce){
				return ERR_SEM_NEDEF;
			}
		}

		if (error != ERR_OK){
			return error;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_LZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		/* zpracovani parametru
		   pokud je funkce nedefinovana, vytvorim novou
		   tabulku symbolu a zavolam funkci pro zpracovani
		   parametru, v opacnem pripade porovnam typy */

		if (insert){
			error = s_param();
			newItem->params = pomParametry.First;
		} else {
			error = s_param();
			if ((PCheck (newItem->params , pomParametry.First)) == false){
				return ERR_SEM_VYRAZ;
			}
		}
		
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_PZ){
			return ERR_SYN;
		}

		//konec s parametry

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		// V pripade, ze dalsi token je strednik,
		// tak ulozim funkci do seznamu a pokracuju
		// v rekurzivnim sestupu

		if (token.stav == S_STREDNIK){
			
			newItem->def = false;
			newItem->typ = typFunkce;
			newItem->nazev = nazevFunkce;

			GInsertLast (&global, newItem);
			newItem = NULL;

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			return s_funkce();
		}

		// pokud nenasleduje strednik, tak musi nasledovat
		// slozena zavorka, jinak je to chyba
		/* vygeneruju si instrukci zacatku funkce, vlozim
		   instrukci, ktera mi pushne parametry jako ramec
		   s hodnotou 1 a zavolam s_stat() */

		if (token.stav == S_SLZ){

			pomFun = newLabel(halda);

			aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
			if (error != ERR_OK){
				return error;
			}
			TRPInit(aktTab);
			ramec_check = 1;

			PInsertTab (newItem->params, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktInstrukce = newInstr(I_START_FUNKCE, pomFun, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;

		} else return ERR_SYN;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_stat();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		// vlozim label pro konec funkce a instrukci pro zruseni ramcu funkce

		aktInstrukce = newInstr(I_KONEC_FUNKCE, pomFun, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		aktInstrukce = newInstr(I_POP_ALL, NULL, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		// ulozim data do seznamu, zrusim pomocnou pasku a vratim
		// funkci s_funkce()

		newItem->typ = typFunkce;
		newItem->nazev = nazevFunkce;
		newItem->paskaZ = pomPaska.First;
		newItem->paskaK = pomPaska.Last;

		if (insert){
			GInsertLast (&global, newItem);
		}
		newItem = NULL;
		ILInit(&pomPaska);
		PInit(&pomParametry);

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_funkce();

	} else { //main

		if (typFunkce != 1){
			return ERR_SYN;
		}

		main_cnt++;

		nazevFunkce = token.data;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_LZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_PZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_SLZ){
			return ERR_SYN;
		}

		tGData *newItem;
		newItem = newMalloc(sizeof(struct tGlobalData));
		if (error != ERR_OK){
			return error;
		}

		pomFun = newLabel(halda);

		aktInstrukce = newInstr(I_START, pomFun, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		newItem->params = NULL;
		newItem->def = true;
		newItem->typ = 1;
		newItem->nazev = nazevFunkce;

		aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
		TRPInit(aktTab);
		ramec_check = 1;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		
		error = s_stat();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_EOF){
			return ERR_SYN;
		}

		aktInstrukce = newInstr(I_KONEC, pomFun, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		newItem->paskaZ = pomPaska.First;
		newItem->paskaK = pomPaska.Last;

		GInsertLast (&global, newItem);
		newItem = NULL;
		ILInit(&pomPaska);
		PInit(&pomParametry);

		return error;
	}
}

tError s_stat(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_IF){ /////////////////////////////////////////////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
		TRPInit(aktTab);
		ramec_check = 2;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_LZ){
			return ERR_SYN;
		}

		error = expr(&randomNull);
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_SLZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		char *pomIf = newLabel(halda);
		aktInstrukce = newInstr(I_IF, pomIf, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		error = s_stat();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		aktInstrukce = newInstr(I_POP_FRAME, NULL, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		return s_else(pomIf);

	} else if (token.stav == S_RETURN){ /////////////////////////////////////////////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = expr(&randomNull);
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		aktInstrukce = newInstr(I_RETURN, pomFun, nazevFunkce, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		return s_stat();

	} else if (token.stav == S_SLZ){ /////////////////////////////////////////////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
		TRPInit(aktTab);
		ramec_check = 2;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_stat();
		if (error != ERR_OK){
			return error;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		return s_stat();

	} else if (token.stav == S_FOR){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_LZ){
			return ERR_SYN;
		}

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
		TRPInit(aktTab);
		ramec_check = 2;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav == S_INT){//..................................................................

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav != S_IDENT){
				return ERR_SYN;
			}

			char *pomIdent = token.data;

			if (TRPSearch(aktTab, token.data) != NULL){
				return ERR_SEM_NEDEF;
			}

			tData *aktData = newMalloc(sizeof(struct tdata));
			if (error != ERR_OK){
				return error;
			}

			aktData->typ = 1;
			aktData->nazev = token.data;
			aktData->def = false;
			aktData->ramec = 0;

			TRPInsert(aktTab, token.data, aktData);

			if (aktTab != NULL){
				TSInsertLast (&tabulkaSymbolu, aktTab);
				char *pom;
				pom = newFrame(halda, aktTab, ramec_check);

				aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);

				aktTab = NULL;
			}

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav == S_PRIR){
				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

				char *lastExp = NULL;

				error = expr(&lastExp);

				aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);
			}		

			if (token.stav != S_STREDNIK){
				return ERR_SYN;
			}

		} else if (token.stav == S_DOUBLE){//..................................................................

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav != S_IDENT){
				return ERR_SYN;
			}

			char *pomIdent = token.data;

			if (TRPSearch(aktTab, token.data) != NULL){
				return ERR_SEM_NEDEF;
			}

			tData *aktData = newMalloc(sizeof(struct tdata));
			if (error != ERR_OK){
				return error;
			}

			aktData->typ = 2;
			aktData->nazev = token.data;
			aktData->def = false;
			aktData->ramec = 0;

			TRPInsert(aktTab, token.data, aktData);

			if (aktTab != NULL){
				TSInsertLast (&tabulkaSymbolu, aktTab);
				char *pom;
				pom = newFrame(halda, aktTab, ramec_check);

				aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);

				aktTab = NULL;
			}

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav == S_PRIR){
				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

				char *lastExp = NULL;

				error = expr(&lastExp);

				aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);
			}		

			if (token.stav != S_STREDNIK){
				return ERR_SYN;
			}
					
		} else if (token.stav == S_STRING){//..................................................................

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav != S_IDENT){
				return ERR_SYN;
			}

			char *pomIdent = token.data;

			if (TRPSearch(aktTab, token.data) != NULL){
				return ERR_SEM_NEDEF;
			}

			tData *aktData = newMalloc(sizeof(struct tdata));
			if (error != ERR_OK){
				return error;
			}

			aktData->typ = 3;
			aktData->nazev = token.data;
			aktData->def = false;
			aktData->ramec = 0;

			TRPInsert(aktTab, token.data, aktData);

			if (aktTab != NULL){
				TSInsertLast (&tabulkaSymbolu, aktTab);
				char *pom;
				pom = newFrame(halda, aktTab, ramec_check);

				aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);

				aktTab = NULL;
			}

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav == S_PRIR){
				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

				char *lastExp = NULL;

				error = expr(&lastExp);

				aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);
			}		

			if (token.stav != S_STREDNIK){
				return ERR_SYN;
			}
			
		} else if (token.stav == S_AUTO){//..................................................................

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav != S_IDENT){
				return ERR_SYN;
			}

			char *pomIdent = token.data;

			if (TRPSearch(aktTab, token.data) != NULL){
				return ERR_SEM_NEDEF;
			}

			tData *aktData = newMalloc(sizeof(struct tdata));
			if (error != ERR_OK){
				return error;
			}

			aktData->typ = 6;
			aktData->nazev = token.data;
			aktData->def = false;
			aktData->ramec = 0;

			TRPInsert(aktTab, token.data, aktData);

			if (aktTab != NULL){
				TSInsertLast (&tabulkaSymbolu, aktTab);
				char *pom;
				pom = newFrame(halda, aktTab, ramec_check);

				aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);

				aktTab = NULL;
			}

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			if (token.stav == S_PRIR){
				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}
				
				char *lastExp = NULL;

				error = expr(&lastExp);

				aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);

			} else return ERR_SEM_TYP;	

			if (token.stav != S_STREDNIK){
				return ERR_SYN;
			}
			
		} else return ERR_SYN;//................................................................................

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		char *pomFor = newLabel(halda);

		aktInstrukce = newInstr(I_FOR, pomFor, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		error = expr(&randomNull);
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		aktInstrukce = newInstr(I_FOR_VYRAZ, pomFor, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		char *pomIdent2 = token.data;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_PRIR){
			return ERR_SYN;
		}

		token.data = "(";
		token.stav = S_LZ;

		char *lastExp = NULL;

		error = expr(&lastExp);

		aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent2);
		ILInsertLast(&pomPaska, aktInstrukce);

		aktInstrukce = newInstr(I_FOR_START, pomFor, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		if (token.stav != S_SLZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_stat();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		aktInstrukce = newInstr(I_FOR_KONEC, pomFor, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		aktInstrukce = newInstr(I_POP_FRAME, NULL, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		return s_stat();

	} else if (token.stav == S_IDENT){ /////////////////////////////////////////////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		char *pomIdent = token.data;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_PRIR){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav == S_IDENT){

			if ((GFind (&global, token.data)) || (strcmp(nazevFunkce, token.data) == 0)){ 

				char *pomIdent2 = token.data;

				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

				if (token.stav != S_LZ) return ERR_SYN;

				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

				error = s_arg();

				if (token.stav != S_PZ) return ERR_SYN;

				char *pom = newArgs(halda, pomParametry.First);

				aktInstrukce = newInstr(I_FUNKCE, pomIdent2, pom, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);

				aktInstrukce = newInstr(I_PRIR, NULL, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);

				token = getToken();
				if (token.stav == S_CHYBAVS){
					return error;
				}

			} else {
				char *lastExp = NULL;

				error = expr(&lastExp);

				aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
				ILInsertLast(&pomPaska, aktInstrukce);

			} 
		} else {
			char *lastExp = NULL;

			error = expr(&lastExp);

			aktInstrukce = newInstr(I_PRIR, lastExp, NULL, pomIdent);
			ILInsertLast(&pomPaska, aktInstrukce);

		}
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();

	} else if (token.stav == S_INT){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		if (aktTab == NULL){
			aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
			if (error != ERR_OK){
				return error;
			}
			TRPInit(aktTab);
			ramec_check = 3;
		}

		if (TRPSearch(aktTab, token.data) != NULL){
			return ERR_SEM_NEDEF;
		}

		tData *aktData = newMalloc(sizeof(struct tdata));
		if (error != ERR_OK){
			return error;
		}

		aktData->typ = 1;
		aktData->nazev = token.data;
		aktData->def = false;
		aktData->ramec = 0;

		TRPInsert(aktTab, token.data, aktData);

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_STREDNIK){
			error = s_exp();
		}	
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();

	} else if (token.stav == S_DOUBLE){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		if (aktTab == NULL){
			aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
			if (error != ERR_OK){
				return error;
			}
			TRPInit(aktTab);
			ramec_check = 3;
		}

		if (TRPSearch(aktTab, token.data) != NULL){
			return ERR_SEM_NEDEF;
		}

		tData *aktData = newMalloc(sizeof(struct tdata));
		if (error != ERR_OK){
			return error;
		}

		aktData->typ = 2;
		aktData->nazev = token.data;
		aktData->def = false;
		aktData->ramec = 0;

		TRPInsert(aktTab, token.data, aktData);

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_STREDNIK){
			error = s_exp();
		}		
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();
		
	} else if (token.stav == S_STRING){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		if (aktTab == NULL){
			aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
			if (error != ERR_OK){
				return error;
			}
			TRPInit(aktTab);
			ramec_check = 3;
		}

		if (TRPSearch(aktTab, token.data) != NULL){
			return ERR_SEM_NEDEF;
		}

		tData *aktData = newMalloc(sizeof(struct tdata));
		if (error != ERR_OK){
			return error;
		}

		aktData->typ = 3;
		aktData->nazev = token.data;
		aktData->def = false;
		aktData->ramec = 0;

		TRPInsert(aktTab, token.data, aktData);

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_STREDNIK){
			error = s_exp();
		}	
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();

	} else if (token.stav == S_AUTO){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (GFind (&global, token.data)){
			error = s_arg();
		} else error = s_exp();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();
	
	} else if (token.stav == S_CIN){/////////////////////////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_cin();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();

	} else if (token.stav == S_COUT){////////////////////////////////////////////////////////

		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, ramec_check);

			aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
			ILInsertLast(&pomPaska, aktInstrukce);

			aktTab = NULL;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_cout();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_STREDNIK){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_stat();

	} else return error; /////////////////////////////////////////////////////////////////////////////////////////////////
}

tError s_else(char *pomIE){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_ELSE){
		return ERR_SYN;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav != S_SLZ){
		return ERR_SYN;
	}

	aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}
	TRPInit(aktTab);
	ramec_check = 2;

	aktInstrukce = newInstr(I_ELSE, pomIE, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	error = s_stat();
	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_SPZ){
		return ERR_SYN;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (aktTab != NULL){
		TSInsertLast (&tabulkaSymbolu, aktTab);
		char *pom;
		pom = newFrame(halda, aktTab, ramec_check);

		aktInstrukce = newInstr(I_PUSH_FRAME, pom, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		aktTab = NULL;
	}

	aktInstrukce = newInstr(I_POP_FRAME, NULL, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	aktInstrukce = newInstr(I_IFELSE, pomIE, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	return s_stat();

}

tError s_exp(){

	if (error != ERR_OK){
		return error;
	}

	//verze pro test

	/*aktInstrukce = newInstr(I_EXP_LAST, NULL, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	if (token.stav == S_STREDNIK || token.stav == S_PZ){
		return ERR_SYN;  //pozor na prazdny vyraz, potreba vyresit!!!
	}

	while (1){

		if (token.stav == S_STREDNIK || token.stav == S_PZ){
			return error;
		} else if (token.stav == S_EOF){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
	}*/



	/*if (token.stav != S_TYPINT){
		return ERR_SYN;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}*/

	return error;
}

tError s_term(){

	if (error != ERR_OK){
		return error;
	}

	return error;

}

tError s_param(){

	if (error != ERR_OK){
		return error;
	}

	// ulozim si typ parametru

	int typPar;

	if (token.stav == S_INT) typPar = 1;
	else if (token.stav == S_DOUBLE) typPar = 2;
	else if (token.stav == S_STRING) typPar = 3;
	else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	// vlozim parametr do seznamu

	if (token.stav != S_IDENT){
		return ERR_SYN;
	}

	PInit(&pomParametry);
	PInsertLast (&pomParametry, typPar, token.data);

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_param2();
}

tError s_param2(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_CARKA){
		return error;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	// ulozim si typ parametru

	int typPar;

	if (token.stav == S_INT) typPar = 1;
	else if (token.stav == S_DOUBLE) typPar = 2;
	else if (token.stav == S_STRING) typPar = 3;
	else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	// vlozim parametr do seznamu

	if (token.stav != S_IDENT){
		return ERR_SYN;
	}

	PInsertLast (&pomParametry, typPar, token.data);

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_param2();
}

tError s_arg(){

	if (error != ERR_OK){
		return error;
	}

	PInit(&pomParametry);

	if (token.stav == S_IDENT){
		
		PInsertArg (&pomParametry, 0, token.data, 0, 0, NULL);

	} else if (token.stav == S_TYPINT){
		
		int x = atoi(token.data);
		PInsertArg (&pomParametry, 1, NULL, x, 0, NULL);
		
	} else if (token.stav == S_TYPDOUBLE){
		
		double y;
		sscanf(token.data, "%lf", &y);
		PInsertArg (&pomParametry, 2, NULL, 0, y, NULL);
		
	} else if (token.stav == S_TYPSTRING){
		
		PInsertArg (&pomParametry, 3, NULL, 0, 0, token.data);
		
	} else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}
	return s_arg2();
}

tError s_arg2(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_CARKA){
		return error;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav == S_IDENT){
		
		PInsertArg (&pomParametry, 0, token.data, 0, 0, NULL);

	} else if (token.stav == S_TYPINT){
		
		int x = atoi(token.data);
		PInsertArg (&pomParametry, 1, NULL, x, 0, NULL);
		
	} else if (token.stav == S_TYPDOUBLE){
		
		double y;
		sscanf(token.data, "%lf", &y);
		PInsertArg (&pomParametry, 2, NULL, 0, y, NULL);
		
	} else if (token.stav == S_TYPSTRING){
		
		PInsertArg (&pomParametry, 3, NULL, 0, 0, token.data);
		
	} else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}
	return s_arg2();

}

tError s_cin(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_PPRIR){
		return ERR_SYN;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav != S_IDENT){
		return ERR_SYN;
	}

	aktInstrukce = newInstr(I_CIN, token.data, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_cin2();
}

tError s_cout(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_LPRIR){
		return ERR_SYN;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav == S_IDENT){

		aktInstrukce = newInstr(I_COUT, token.data, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	} else if (token.stav == S_TYPINT){

		char *pom = newInt(halda, token.data);

		aktInstrukce = newInstr(I_COUT, pom, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	} else if (token.stav == S_TYPDOUBLE){

		char *pom = newDouble(halda, token.data);

		aktInstrukce = newInstr(I_COUT, pom, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	} else if (token.stav == S_TYPSTRING){

		char *pom = newStr(halda, token.data);

		aktInstrukce = newInstr(I_COUT, pom, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	} else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_cout2();
}

tError s_cin2(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_PPRIR){
		return error;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav != S_IDENT){
		return ERR_SYN;
	}

	aktInstrukce = newInstr(I_CIN, token.data, NULL, NULL);
	ILInsertLast(&pomPaska, aktInstrukce);

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_cin2();
}

tError s_cout2(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav != S_LPRIR){
		return error;
	}

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	if (token.stav == S_IDENT){

		aktInstrukce = newInstr(I_COUT, token.data, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	/*} else if (token.stav == S_TYPINT){

	} else if (token.stav == S_TYPDOUBLE){*/

	} else if (token.stav == S_TYPSTRING){

		char *pom = newStr(halda, token.data);

		aktInstrukce = newInstr(I_COUT, pom, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

	} else return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS){
		return error;
	}

	return s_cout2();
}

//funkce pro ukladani do TS

char *newFrame(tTabulka* halda, tTabulka *newItem, int ramec){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->hodnota = (void *) newMalloc(sizeof(tTabulka));
	dataFrame->hodnota->tabulka = newItem;
	dataFrame->nazev = key;
	dataFrame->typ = 5;
	dataFrame->def = true;
	dataFrame->ramec = ramec;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}

char *newArgs(tTabulka* halda, tParamElemPtr newItem){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->hodnota = (void *) newMalloc(sizeof(struct tParamElem));
	dataFrame->hodnota->args = newItem;
	dataFrame->nazev = key;
	dataFrame->typ = 6;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}

char *newLabel(tTabulka* halda){

	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->typ = 4;
	dataFrame->def = true;
	dataFrame->ramec = 0;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}