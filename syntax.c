#include "syntax.h"

int main_cnt = 0;
bool for_check = false;
bool isBoss = false;
char *nazevFunkce;
tError error;

tInstrList pomPaska; //pomocna paska, ktera se pak ulozi do dat aktualni funkce
tGList global; //seznam globalnich funkci
tTSList tabulkaSymbolu; //seznam globalnich promennych

tTabulka *pomTab; //zatim nevim na co
tTabulka *aktTab; //aktualni tabulka
tTabulka *halda; //halda pomocnych promennych
tGData *aktFunkce; //data aktualne zpracovavane funkce
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

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_DOUBLE) {

	} else if (token.stav == S_INT) {

	} else if (token.stav == S_STRING) {

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

		error = s_param();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_PZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav == S_SLZ){

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			error = s_stat();
			if (error != ERR_OK){
				return error;
			}

		} else if (token.stav == S_STREDNIK) {

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			return s_funkce();

		} else return ERR_SYN;

		if (token.stav != S_SPZ){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		return s_funkce();

	} else { //main

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

		aktInstrukce = newInstr(I_START, NULL, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		newItem->params = NULL;
		newItem->def = true;
		newItem->nazev = nazevFunkce;

		aktFunkce = newItem;

		isBoss = true;
		aktTab = (tTabulka *) newMalloc(sizeof(tTabulka));
		if (error != ERR_OK){
			return error;
		}

		TRPInit(aktTab);

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

		aktInstrukce = newInstr(I_KONEC, NULL, NULL, NULL);
		ILInsertLast(&pomPaska, aktInstrukce);

		newItem->paskaZ = pomPaska.First;
		newItem->paskaK = pomPaska.Last;

		GInsertLast (&global, newItem);
		newItem = NULL;
		ILInit(&pomPaska);

		return error;
	}
}

tError s_stat(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_IF){ /////////////////////////////////////////////////////////////////////////////////////////////////

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

		error = s_exp();
		if (error != ERR_OK){
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

		return s_else();

	} else if (token.stav == S_RETURN){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_exp();
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

		return error;

	} else if (token.stav == S_SLZ){ /////////////////////////////////////////////////////////////////////////////////////////////////

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

		return s_stat();

	} else if (token.stav == S_FOR){ /////////////////////////////////////////////////////////////////////////////////////////////////

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

		if (token.stav == S_INT){

		} else if (token.stav == S_DOUBLE){
		
		} else if (token.stav == S_STRING){

		} else if (token.stav == S_AUTO){

		} else return ERR_SYN;

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

		for_check = true;

		error = s_def();
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

		error = s_exp();
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

		if (token.stav != S_IDENT){
			return ERR_SYN;
		}

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_prir();
		if (error != ERR_OK){
			return error;
		}

		if (token.stav != S_PZ){
			return ERR_SYN;
		}

		for_check = false;

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

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

		return s_stat();

	} else if (token.stav == S_IDENT){ /////////////////////////////////////////////////////////////////////////////////////////////////

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_prir();
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

		TRPInsert(aktTab, token.data, aktData);

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_def();
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

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_def();
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

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		error = s_def();
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

		error = s_def();
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

		pomTab = aktTab;
		if (aktTab != NULL){
			TSInsertLast (&tabulkaSymbolu, aktTab);
			char *pom;
			pom = newFrame(halda, aktTab, isBoss);
			if (isBoss){
				isBoss = false;
			}

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
			pom = newFrame(halda, aktTab, isBoss);
			if (isBoss){
				isBoss = false;
			}

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

tError s_else(){

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

	return s_stat();

}

tError s_def(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_STREDNIK && for_check == false){

		return error;

	} else if (token.stav == S_STREDNIK && for_check == true){

		return error;

	} else if (token.stav == S_SPZ && for_check == true){

		return error;

	} else return s_prir();
}

tError s_prir(){

	if (error != ERR_OK){
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

		//prohledat tabulku, pokud je funkce, tak pokracuj, jinak EXP

		//verze pro test

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}

		if (token.stav == S_LZ){

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			error = s_arg();
			if (error != ERR_OK){
				return error;
			}

			if (token.stav != S_PZ){
				return ERR_SYN;
			}

			token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}

			return error;

		} else if (token.stav == S_STREDNIK && for_check){

			return error;

		} else if (token.stav == S_STREDNIK && for_check == false){

			/*token = getToken();
			if (token.stav == S_CHYBAVS){
				return error;
			}*/

			return error;

		} else if (token.stav == S_PZ && for_check){

			return error;

		} else return s_exp();

	} else return s_exp();
}

tError s_exp(){

	if (error != ERR_OK){
		return error;
	}

	//verze pro test

	if (token.stav == S_STREDNIK || token.stav == S_PZ){
		return ERR_SYN;
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
	}

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

	if (token.stav != S_INT &&
		token.stav != S_DOUBLE &&
		token.stav != S_STRING &&
		token.stav != S_AUTO){

		return ERR_SYN;
	}

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

	if (token.stav != S_INT &&
		token.stav != S_DOUBLE &&
		token.stav != S_STRING &&
		token.stav != S_AUTO){

		return ERR_SYN;
	}

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

	return s_param2();
}

tError s_arg(){

	if (error != ERR_OK){
		return error;
	}

	if (token.stav == S_IDENT){

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();

	} else if (token.stav == S_TYPINT){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else if (token.stav == S_TYPDOUBLE){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else if (token.stav == S_TYPSTRING){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else return ERR_SYN;
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

		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();

	} else if (token.stav == S_TYPINT){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else if (token.stav == S_TYPDOUBLE){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else if (token.stav == S_TYPSTRING){
		
		token = getToken();
		if (token.stav == S_CHYBAVS){
			return error;
		}
		return s_arg2();
		
	} else return ERR_SYN;

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

char *newFrame(tTabulka* halda, tTabulka *newItem, bool boss){
  
	char *key;
	key = generateVar();
	tData *dataFrame = newMalloc(sizeof(tData));
	dataFrame->hodnota = (void *) newMalloc(sizeof(tTabulka));
	dataFrame->hodnota->tabulka = newItem;
	dataFrame->nazev = key;
	dataFrame->typ = 5;
	dataFrame->def = true;
	dataFrame->boss = boss;
	
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
	dataFrame->boss = false;
	
	TRPInsert(halda, key, dataFrame);
	
	return key;
}