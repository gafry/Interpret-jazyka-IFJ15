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

#include "vest.h"

tError error;
tToken token;
tInstrList pomPaska;
tInstrukce aktInstrukce;
tTabulka *halda;

/* udelam instrukce, ve kterych zavolam dane funkce,
   ulozim vysledek do pomocne promenne, vytvorim novou
   v halde, prejdu na I_PRIR a predam vysledek do ard1 */

tError vest(int i){

	token = getToken();
	if (token.stav == S_CHYBAVS) return error;

	if (token.stav != S_LZ) return ERR_SYN;

	token = getToken();
	if (token.stav == S_CHYBAVS) return error;

	char *pomAdr1 = NULL;
	char *pomAdr2 = NULL;
	char *pomAdr3 = NULL;

	switch(i){

		case 1:
		case 5:

			if (token.stav == S_IDENT){
				pomAdr1 = token.data;
			} else if (token.stav == S_TYPSTRING){
				pomAdr1 = newStr(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_PZ) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (i == 1){
				aktInstrukce = newInstr(I_LENGTH, pomAdr1, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);
			} else {
				aktInstrukce = newInstr(I_SORT, pomAdr1, NULL, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);
			}

			break;

		case 2:

			if (token.stav == S_IDENT){
				pomAdr1 = token.data;
			} else if (token.stav == S_TYPSTRING){
				pomAdr1 = newStr(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_CARKA) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav == S_IDENT){
				pomAdr2 = token.data;
			} else if (token.stav == S_TYPINT){
				pomAdr2 = newInt(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_CARKA) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav == S_IDENT){
				pomAdr3 = token.data;
			} else if (token.stav == S_TYPINT){
				pomAdr3 = newInt(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_PZ) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			aktInstrukce = newInstr(I_SUBSTR, pomAdr1, pomAdr2, pomAdr3);
			ILInsertLast(&pomPaska, aktInstrukce);

			break;

		case 3:
		case 4:

			if (token.stav == S_IDENT){
				pomAdr1 = token.data;
			} else if (token.stav == S_TYPSTRING){
				pomAdr1 = newStr(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_CARKA) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav == S_IDENT){
				pomAdr2 = token.data;
			} else if (token.stav == S_TYPSTRING){
				pomAdr2 = newStr(halda, token.data);
			} else return ERR_SEM_VYRAZ;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (token.stav != S_PZ) return ERR_SYN;

			token = getToken();
			if (token.stav == S_CHYBAVS) return error;

			if (i == 3){
				aktInstrukce = newInstr(I_CONCAT, pomAdr1, pomAdr2, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);
			} else {
				aktInstrukce = newInstr(I_FIND, pomAdr1, pomAdr2, NULL);
				ILInsertLast(&pomPaska, aktInstrukce);
			}

			break;
	}
	return ERR_OK;
}

// jednotlive vestavene funkce krome find - ten se vola primo z ial.c
char *concatVest (char *s1, char *s2){   
	char *newItem;
	newItem = newMalloc((sizeof(char)*(strlen(s1)+2)) + (sizeof(char)*(strlen(s2)+2)));
	if (newItem == NULL) return NULL;  
	strcpy(newItem, s1);
	//newItem = newRealloc(newItem, (sizeof(char)*(strlen(s2)+2)));
	strcpy(&newItem[strlen(newItem)], s2);
    return newItem;
}

int lengthVest (char *s){
	int x;
	x = strlen(s);
	return x;
}

char *sortVest(char *s1){
	char *newItem;
	if (strcmp(s1, "") == 0){
		newItem = newMalloc(sizeof(char));
		newItem[0] = '\0';
		return newItem;
	}
	newItem = newMalloc(sizeof(char)*(strlen(s1)+2));
	if (newItem == NULL) return NULL;
	strcpy(newItem, s1);
	shellSort(newItem);
	return newItem;
}

char *substrVest(char *s1, int i1, int i2){
	char *newItem;
	int delka = strlen(s1);
	if (delka == i1 || i2 == 0){
		newItem = newMalloc(sizeof(char));
		newItem[0] = '\0';
	} else if (delka < i1 || i2 < 0){
		error = ERR_OST;
		newItem = NULL;
	} else {
		newItem = newMalloc(sizeof(char)*(i2+1));
		for (int i=0; i<i2; i++){
			if (s1[i1+i] == '\0'){
				newItem[i] = '\0';
				break;
			} else {
				newItem[i] = s1[i1+i];
				newItem[i+1] = '\0';
			}
		}
	}
	return newItem;
}