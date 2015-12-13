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

#include "interpret.h"

tInstrList globalniPaska;
tGList global;
tError error;
tStack ramce;
tTabulka *halda;
tTRPPolozka *pom;
tTRPPolozka *pom2;
tTRPPolozka *pom3;
tGData *pomData;
tParamElemPtr parametry;
tParamElemPtr argumenty;

char *pomName;
int c;
int check = 0;
int expCheck = 0;
bool pomBool;
bool cinBool = false;
bool pomReturn;

tError preloz(){

	ZInit(&ramce);
	ILInit(&globalniPaska);
	GCopyMain (&global, &globalniPaska);
	ILFirst(&globalniPaska);

	while (1){
//printf("\ncislo : %d\n", globalniPaska.Act->instr.op);
		switch (globalniPaska.Act->instr.op){

			/* vestavene funkce vyhledaji sve promenne v ramci, pak s nima 
			   zavolaji dane funkce a vysledek ulozi do haldy */

			case I_LENGTH:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				else if (pom->data->typ != 3) return ERR_SEM_VYRAZ;

				int newLength = lengthVest(pom->data->hodnota->s);
				char *newLength2 = newInt2(halda, newLength);
				if (error != ERR_OK) return error;

				globalniPaska.Act->rptr->instr.adr1 = newLength2;
				globalniPaska.Act = globalniPaska.Act->rptr;

				break;

			case I_SORT:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				else if (pom->data->typ != 3) return ERR_SEM_VYRAZ;

				char *newSort = sortVest(pom->data->hodnota->s);
				if (error != ERR_OK) return error;
				char *newSort2 = newStr(halda, newSort);
				if (error != ERR_OK) return error;

				globalniPaska.Act->rptr->instr.adr1 = newSort2;
				globalniPaska.Act = globalniPaska.Act->rptr;

				break;

			case I_CONCAT:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				else if (pom->data->typ != 3) return ERR_SEM_VYRAZ;
				pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
				if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
				else if (pom2->data->typ != 3) return ERR_SEM_VYRAZ;

				char *newConcat = concatVest(pom->data->hodnota->s, pom2->data->hodnota->s);
				if (error != ERR_OK) return error;
				char *newConcat2 = newStr(halda, newConcat);
				if (error != ERR_OK) return error;

				globalniPaska.Act->rptr->instr.adr1 = newConcat2;
				globalniPaska.Act = globalniPaska.Act->rptr;

				break;

			case I_FIND:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				else if (pom->data->typ != 3) return ERR_SEM_VYRAZ;
				pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
				if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
				else if (pom2->data->typ != 3) return ERR_SEM_VYRAZ;

				int newFind = boyer_moore(pom->data->hodnota->s, pom2->data->hodnota->s);
				if (strcmp(pom2->data->hodnota->s, "") == 0) newFind = 0;
				char *newFind2 = newInt2(halda, newFind);
				if (error != ERR_OK) return error;

				globalniPaska.Act->rptr->instr.adr1 = newFind2;
				globalniPaska.Act = globalniPaska.Act->rptr;

				break;

			case I_SUBSTR:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				else if (pom->data->typ != 3) return ERR_SEM_VYRAZ;
				pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
				if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
				else if (pom2->data->typ != 1) return ERR_SEM_VYRAZ;
				pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
				if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;
				else if (pom3->data->typ != 1) return ERR_SEM_VYRAZ;

				if (pom2->data->hodnota->i < 0 || pom3->data->hodnota->i < 0) return ERR_OST;

				char *newSubstr = substrVest(pom->data->hodnota->s, pom2->data->hodnota->i, pom3->data->hodnota->i);
				if (error != ERR_OK) return error;
				char *newSubtr2 = newStr(halda, newSubstr);
				if (error != ERR_OK) return error;

				globalniPaska.Act->rptr->instr.adr1 = newSubtr2;
				globalniPaska.Act = globalniPaska.Act->rptr;

				break;

			/* zacatek instrukcni pasky */

			case I_START:

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* FOR funguje tak, ze nejdriv zpracuje prirazeni, ulozi promennou
			   do tabulky symbolu a pushne ramec, pote dojde na instrukci
			   I_FOR_VYRAZ, pokud je <EXP> true, prejdu na I_FOR_START, v opacnem 
			   pripade prejdu na I_FOR_KONEC
			   Kdyz se dostanu na I_FOR_START, tak se dostanu do <STAT>, projdu vsechny
			   instrukce a az se dostanu na I_FOR_KONEC, tak prejdu na I_FOR_VYRAZ, kde 
			   opet zkontroluju hodnotu <EXP> a proces se opakuje */

			case I_FOR:

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_FOR_VYRAZ:

				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1;
				globalniPaska.Pom = globalniPaska.Act;
				globalniPaska.Pom = globalniPaska.Act->lptr;
				pom = ZSearch(&ramce, globalniPaska.Pom->instr.vysl);
				if (pom->data->typ != 1) return ERR_SEM_VYRAZ;
				expCheck = pom->data->hodnota->i;
				if (expCheck){
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->rptr;
						if (globalniPaska.Act->instr.op == I_FOR_START){
							globalniPaska.Pom = globalniPaska.Act;
							if (globalniPaska.Pom->instr.adr1 == pomName){
								pomBool = false;
								globalniPaska.Act = globalniPaska.Act->rptr;
							}						
						}
					}
				} else {
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->rptr;
						if (globalniPaska.Act->instr.op == I_FOR_KONEC){
							globalniPaska.Pom = globalniPaska.Act;
							if (globalniPaska.Pom->instr.adr1 == pomName){
								pomBool = false;
								globalniPaska.Act = globalniPaska.Act->rptr;
							}						
						}
					}
				}
				if (error != ERR_OK) return error;
				break;

			case I_FOR_START:

				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1;
				while (pomBool){
					globalniPaska.Act = globalniPaska.Act->lptr;
					if (globalniPaska.Act->instr.op == I_FOR){
						if (globalniPaska.Act->instr.adr1 == pomName){
							pomBool = false;
						}						
					}
				}
				if (error != ERR_OK) return error;
				break;

			case I_FOR_KONEC:
			
				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1;
				while (pomBool){
					globalniPaska.Act = globalniPaska.Act->lptr;
					if (globalniPaska.Act->instr.op == I_FOR_VYRAZ){
						globalniPaska.Pom = globalniPaska.Act;
						if (globalniPaska.Pom->instr.adr1 == pomName){
							pomBool = false;
							globalniPaska.Act = globalniPaska.Act->rptr;
						}						
					}
				}
				if (error != ERR_OK) return error;
				break;

			/* pushne tabulku na ramec, kterou ma ulozenou na adr1 */

			case I_PUSH_FRAME:

				pom = TRPSearch(halda, globalniPaska.Act->instr.adr1);
				if (error != ERR_OK) return error;
				ZPush(&ramce, pom->data->hodnota->tabulka, pom->data->ramec);
				if (error != ERR_OK) return error;

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* popne ramce az po ramce s hodnotou 2 (bloky if, for, {}, ...) */

			case I_POP_FRAME:

				ZPop(&ramce, 2);
				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* popne ramce po hodnotu 1, tzn. vsechny promenne nejake funkce */

			case I_POP_ALL:

				ZPop(&ramce, 1);
				if (error != ERR_OK) return error;

				pomName = globalniPaska.Act->lptr->instr.adr1;
				pomBool = true;
				globalniPaska.Pom = globalniPaska.Act;
				globalniPaska.Act = globalniPaska.Act->rptr;
				while (pomBool){
					globalniPaska.Pom = globalniPaska.Pom->lptr;
					if (globalniPaska.Pom->instr.op == I_START_FUNKCE){
						if (globalniPaska.Pom->instr.adr1 == pomName){
							globalniPaska.Pom = globalniPaska.Pom->lptr;
							pomBool = false;
							globalniPaska.Act->lptr = globalniPaska.Pom->lptr;
							globalniPaska.Pom->lptr->rptr = globalniPaska.Act;
						}
					}
				}

				break;

			/* vypise co ma na adr1 */

			case I_COUT:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
				if (pom->data->typ == 1 && pom->data->def){ //printf("%s....%d\n", pom->data->nazev, pom->data->hodnota->i);
					printf("%d", pom->data->hodnota->i);
				} else if (pom->data->typ == 2 && pom->data->def){  //printf("%s....%g\n", pom->data->nazev, pom->data->hodnota->d);
					printf("%g", pom->data->hodnota->d); //printf("%s....%g\n", pom->data->nazev, pom->data->hodnota->d);
				} else if (pom->data->typ == 3 && pom->data->def){ 
					printf("%s", pom->data->hodnota->s);					
				} else return ERR_SEM_NEDEF;

				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* podivam se jak dopadla podminka, podle toho bud pokracuju
			   dalsi instrukci, nebo jdu na I_ELSE a pokracuju tam */

			case I_IF:

				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1; 
				globalniPaska.Pom = globalniPaska.Act;
				globalniPaska.Pom = globalniPaska.Act->lptr;
				pom = ZSearch(&ramce, globalniPaska.Pom->instr.vysl);
				if (pom->data->typ != 1) return ERR_SEM_VYRAZ;
				expCheck = pom->data->hodnota->i;
				if (expCheck){
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->rptr;
						if (globalniPaska.Act->instr.op == I_IFELSE){
							globalniPaska.Pom = globalniPaska.Act;
							if (globalniPaska.Pom->instr.adr1 == pomName){
								pomBool = false;
							}						
						}
					}
					pomBool = true;
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->lptr;
						if (globalniPaska.Act->instr.op == I_ELSE){
							if (globalniPaska.Act->instr.adr1 == pomName){
								pomBool = false;
								globalniPaska.Act->jump = globalniPaska.Pom;
							}						
						}
					}
					pomBool = true;
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->lptr;
						if (globalniPaska.Act->instr.op == I_IF){
							if (globalniPaska.Act->instr.adr1 == pomName){
								pomBool = false;
							}						
						}
					}
					globalniPaska.Act = globalniPaska.Act->rptr;
				} else { 
					while (pomBool){
						globalniPaska.Act = globalniPaska.Act->rptr;
						if (globalniPaska.Act->instr.op == I_ELSE){//printf("%s .... %s\n", pomName, globalniPaska.Act->instr.adr1);
							if (globalniPaska.Act->instr.adr1 == pomName){
								pomBool = false;
								globalniPaska.Act->jump = NULL;
							}						
						}
					}
				}
				if (error != ERR_OK) return error;
				break;

			case I_ELSE:
				
				if (globalniPaska.Act->jump != NULL){ 
					globalniPaska.Act = globalniPaska.Act->jump;
				} else globalniPaska.Act = globalniPaska.Act->rptr;

				if (error != ERR_OK) return error;
				globalniPaska.Act->jump = NULL;
				break;

			case I_IFELSE:

				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* podivam se jak dopadlo vyhodnoceni vyrazu, pak zjistim jestli je 
			   volana funkce main() nebo jina a podle toho vratim hodnotu tak, ze
			   ji ulozim na adr1 instrukce I_PRIR */

			case I_RETURN:

				pomReturn = true;
				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1;
				globalniPaska.Pom = globalniPaska.Act->lptr;
				while (pomBool){
					globalniPaska.Act = globalniPaska.Act->rptr;
					if (globalniPaska.Act->instr.op == I_KONEC || globalniPaska.Act->instr.op == I_KONEC_FUNKCE){
						if (globalniPaska.Act->instr.adr1 == pomName){
							pomBool = false;
						}						
					}
				}
				if (globalniPaska.Act->instr.op != I_KONEC){
					if (globalniPaska.Pom->instr.op == I_EXP){
						char *noveId = NULL; 
						pom = ZSearch(&ramce, globalniPaska.Pom->instr.vysl);
						if (pom == NULL) return ERR_SEM_NEDEF;
						else if (pom->data->def == false) return ERR_NEINIT;
						if (pom->data->typ == 1){							
							noveId = newInt2(halda, pom->data->hodnota->i);
						} else if (pom->data->typ == 2){
							noveId = newDouble2(halda, pom->data->hodnota->d);
						} else if (pom->data->typ == 3){
							noveId = newStr(halda, pom->data->hodnota->s);
						}
						globalniPaska.Act->rptr->rptr->instr.adr1 = noveId;
					} else {
						globalniPaska.Act->rptr->rptr->instr.adr1 = globalniPaska.Pom->instr.vysl;
					}
				} else {
					pom = ZSearch(&ramce, globalniPaska.Pom->instr.vysl);
					if (pom == NULL) return ERR_SEM_NEDEF;
					else if (pom->data->def == false) return ERR_NEINIT;
					if (pom->data->typ == 3) return ERR_SEM_VYRAZ;
				}
				if (error != ERR_OK) return error;
				break;

			/* V casti 1 zkontroluju, zda sedi typy parametru a argumentu, pokud je argument
				   identifikator, tak ho najdu a pak az zkontroluju typ.
				   V casti 2 nakopiruju instrukce dane funkce do globalni pasky na aktualni pozici
				   V casti 3 predam nazev funkce a odkaz na argumenty funkce instrukci I_START
				   V casti 4 prejdu na dalsi instrukci */

			case I_FUNKCE:

				//CAST 1
				pomData = GFindData (&global, globalniPaska.Act->instr.adr1); 
				parametry = pomData->params;
				pom = TRPSearch(halda, globalniPaska.Act->instr.adr2);
				argumenty = pom->data->hodnota->args;
				if (error != ERR_OK) return error;
				while (1){  //printf("%s : %d ... %s : %d\n", parametry->data->nazev, parametry->data->typ, argumenty->data->nazev, parametry->data->typ);

					if (parametry == NULL){
						if (argumenty == NULL){
							break;
						} else return ERR_SEM_VYRAZ;
					} else if (argumenty == NULL){
						if (parametry != NULL){
							return ERR_SEM_VYRAZ;
						}
					}

					if (argumenty->data->typ == 0){ 
						pom2 = ZSearch(&ramce, argumenty->data->nazev); 
						if (pom2 == NULL) return ERR_SEM_NEDEF;
						else if (pom2->data->def == false) return ERR_NEINIT; 
						else if (pom2->data->typ != parametry->data->typ) return ERR_SEM_VYRAZ;
					} else if (parametry->data->typ != argumenty->data->typ) return ERR_SEM_VYRAZ;
					parametry = parametry->rptr;
					argumenty = argumenty->rptr;
				}
				if (error != ERR_OK) return error;

				//CAST 2
				tInstrElemPtr dalsiInstrukce = globalniPaska.Act->rptr;
				tInstrElemPtr pomocnaInstrukce = pomData->paskaZ;
				tInstrElemPtr pomocnaInstrukce2 = globalniPaska.Act;
				struct tInstrElem *newItem;
				while (1){ 
					if (pomocnaInstrukce == NULL){
						break;
					}
//printf("%d ... %s ... %s ... %s\n", pomocnaInstrukce->instr.op, pomocnaInstrukce->instr.adr1, pomocnaInstrukce->instr.adr2, pomocnaInstrukce->instr.vysl);
					newItem = newMalloc(sizeof(struct tInstrElem));
					if (error != ERR_OK) return error;

					newItem->instr.op = pomocnaInstrukce->instr.op;
					newItem->instr.adr1 = pomocnaInstrukce->instr.adr1;
					newItem->instr.adr2 = pomocnaInstrukce->instr.adr2;
					newItem->instr.vysl = pomocnaInstrukce->instr.vysl;

					newItem->lptr = pomocnaInstrukce2;
					newItem->rptr = NULL;
					pomocnaInstrukce2->rptr = newItem;

					if (pomocnaInstrukce == pomData->paskaK){
						newItem->rptr = dalsiInstrukce;
						dalsiInstrukce->lptr = newItem;
						break;
					}

					pomocnaInstrukce = pomocnaInstrukce->rptr;
					pomocnaInstrukce2 = pomocnaInstrukce2->rptr;
					newItem = NULL;
				}

				//CAST 3
				globalniPaska.Pom = globalniPaska.Act;
				globalniPaska.Pom = globalniPaska.Pom->rptr->rptr;
				globalniPaska.Pom->instr.adr2 = globalniPaska.Act->instr.adr1;
				globalniPaska.Pom->instr.vysl = globalniPaska.Act->instr.adr2;

				//CAST 4
				globalniPaska.Act = globalniPaska.Act->rptr; //printf("%s\n", "KONEC_HLAVICKY");
				break;

			case I_START_FUNKCE:

				pomData = GFindData (&global, globalniPaska.Act->instr.adr2); 
				parametry = pomData->params;
				pom = TRPSearch(halda, globalniPaska.Act->instr.vysl);
				argumenty = pom->data->hodnota->args;

				while (1){  //printf("%s : %d ... %s : %d\n", parametry->data->nazev, parametry->data->typ, argumenty->data->nazev, argumenty->data->typ);

					if ((parametry == NULL) && (argumenty == NULL)){
						break;
					}

					pom2 = ZSearch(&ramce, parametry->data->nazev); 
					if (pom2 == NULL) return ERR_SEM_NEDEF; 
					pom2->data->def = true;

					if (argumenty->data->typ == 1){ 
						pom2->data->hodnota = newMalloc(sizeof(tHodnota));
						pom2->data->hodnota->i = argumenty->data->hodnota->i;
					} else if (argumenty->data->typ == 2){
						pom2->data->hodnota = newMalloc(sizeof(tHodnota));
						pom2->data->hodnota->d = argumenty->data->hodnota->d;
					} else if (argumenty->data->typ == 3){
						pom2->data->hodnota = newMalloc(sizeof(argumenty->data->hodnota->s));
						pom2->data->hodnota->s = argumenty->data->hodnota->s;
					} else if (argumenty->data->typ == 0){ 
						pom3 = ZRekurzeSearch(&ramce, argumenty->data->nazev);
						if (pom2->data->typ == 1){
							pom2->data->hodnota = newMalloc(sizeof(tHodnota));
							pom2->data->hodnota->i = pom3->data->hodnota->i;
						} else if (pom2->data->typ == 2){
							pom2->data->hodnota = newMalloc(sizeof(tHodnota));
							pom2->data->hodnota->d = pom3->data->hodnota->d;
						} else if (pom2->data->typ == 3){
							pom2->data->hodnota = newMalloc(sizeof(pom3->data->hodnota->s));
							pom2->data->hodnota->i = pom3->data->hodnota->i;
						} else return ERR_INTER;
					}
					if (error != ERR_OK) return error;

					parametry = parametry->rptr;
					argumenty = argumenty->rptr;
				}

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_KONEC_FUNKCE:

				if (pomReturn == false){
					return ERR_NEINIT;
				}
				pomReturn = false;

				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_KONEC:

				if (pomReturn == false){
					return ERR_NEINIT;
				}
				return error;
				break;

			/* v prirazeni zkontroluju typy promennych a podle toho priradim nebo ne */

			case I_PRIR:
//printf("\n%s ... %s ... %s\n", globalniPaska.Act->instr.adr1, globalniPaska.Act->instr.adr2, globalniPaska.Act->instr.vysl);
			
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;             
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL){
					return ERR_SEM_NEDEF; 
				}

                if ((pom->data->typ == 1) && ((pom3->data->typ == 1) || (pom3->data->typ == 7))){
                	pom3->data->hodnota->i = pom->data->hodnota->i;
                    pom3->data->typ = 1; 
                } else if ((pom->data->typ == 1) && (pom3->data->typ == 2)){                       
                    pom3->data->hodnota->d = (double) pom->data->hodnota->i;
                    pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom3->data->typ == 1)){
                	pom3->data->hodnota->i = (int) pom->data->hodnota->d;
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && ((pom3->data->typ == 2) || (pom3->data->typ == 7))){
                	pom3->data->hodnota->d = pom->data->hodnota->d;
                    pom3->data->typ = 2;
                } else if ((pom->data->typ == 3) && ((pom3->data->typ == 3) || (pom3->data->typ == 7))){
                	pom3->data->hodnota->s = newMalloc(sizeof(pom->data->hodnota->s));                       
                    pom3->data->hodnota->s = pom->data->hodnota->s;
                    pom3->data->typ = 3;
                } else return ERR_SEM_VYRAZ;
                pom3->data->def = true;

                if (error != ERR_OK) return error; 
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* pokud pri volani <EXP> je pouze jeden id, tak se ulozi tady */

			case I_EXP:

				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			/* aritmeticke funkce - vyhledaji promenne a provedou vypocet a kontroly
			   s nim spojene */

			case I_ADD:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){ 
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->i = pom->data->hodnota->i + pom2->data->hodnota->i;
               		pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->i + (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d + (double) pom2->data->hodnota->i;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d + (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else {
                	return ERR_SEM_VYRAZ;
                }

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_SUB:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->i = pom->data->hodnota->i - pom2->data->hodnota->i;
               		pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->i - (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d - (double) pom2->data->hodnota->i;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d - (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else {
                	return ERR_SEM_VYRAZ;
                }

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_MUL:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->i = pom->data->hodnota->i * pom2->data->hodnota->i;
               		pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->i * (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d * (double) pom2->data->hodnota->i;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d * (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else {
                	return ERR_SEM_VYRAZ;
                }

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_DIV:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->i = pom->data->hodnota->i / pom2->data->hodnota->i;
               		pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->d == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->i / (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d / (double) pom2->data->hodnota->i;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->d == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d / (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else {
                	return ERR_SEM_VYRAZ;
                }

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_VET:

                pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i > pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d > pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));                   
                    if ((double) pom->data->hodnota->i > (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d > (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) > 0){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_MEN:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i < pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d < pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));                   
                    if ((double) pom->data->hodnota->i < (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d < (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) < 0){                       
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_VETROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i >= pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d >= pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));                   
                    if ((double) pom->data->hodnota->i >= (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d >= (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) >= 0){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_MENROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i <= pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d <= pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));            
                    if ((double) pom->data->hodnota->i <= (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d <= (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) <= 0){                       
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_ROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i == pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d == pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));               
                    if ((double) pom->data->hodnota->i == (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d == (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) == 0){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_NEROV:
			
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;
				else if (pom->data->def == false) return ERR_NEINIT;
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL) return ERR_SEM_NEDEF;
				else if (pom2->data->def == false) return ERR_NEINIT;
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL) return ERR_SEM_NEDEF;
				else if (pom3->data->def == false) return ERR_NEINIT;

                if ((pom->data->typ == 1) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->i != pom2->data->hodnota->i){                            
                    	pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->d != pom2->data->hodnota->d){                            
                        pom3->data->hodnota->i = 1;
                    } else {                            
                        pom3->data->hodnota->i = 0;
                    }
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));                  
                    if ((double) pom->data->hodnota->i != (double) pom2->data->hodnota->d){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if ((double) pom->data->hodnota->d != (double) pom2->data->hodnota->i){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (strcmp(pom->data->hodnota->s, pom2->data->hodnota->s) != 0){                        
                        pom3->data->hodnota->i = 1;
                    } else {                          
                        pom3->data->hodnota->i = 0;
                    }                            
                    pom3->data->typ = 1;
                } else return ERR_SEM_VYRAZ;

                if (error != ERR_OK) return error;
                globalniPaska.Act = globalniPaska.Act->rptr; 
				break;

			/* cin pracuje pri zpracovani int a double podobne jako konecny automat
			   v lexikalnim analyzatoru, kdyz jde o string, tak nacita znaky dokud neni
			   bily znak */

			case I_CIN:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL){
					return ERR_SEM_NEDEF;
				}
				if (pom->data->typ == 1){
					int i = 0;
					bool pomCin = true;
					char *varInt;
					if (!cinBool){
						c = getchar();
					}
					while (isspace(c)){
						c = getchar();
					}
					varInt = newMalloc(sizeof(char)*2);
					if (error != ERR_OK) return error;
					varInt[i] = '\0';
					while (pomCin){		
						if (isspace(c) || c == EOF){
							pomCin = false;
						} else if (isdigit(c)){
							i++;
							varInt = newRealloc(varInt, (sizeof(char)));
							if (error != ERR_OK) return error;
							varInt[i-1] = (char) c;
							varInt[i] = '\0';//printf("\nprom ... %s ... %d\n", pom->data->hodnota->s, c);
							c = getchar();
						} else {
							pomCin = false;
						}
					}
					if (varInt[0] == '\0') return ERR_VSTUP;
					pom->data->hodnota->i = atoi(varInt);
					cinBool = true;
					pom->data->def = true;
					pom->data->typ = 1;
				} else if (pom->data->typ == 2){
					int i = 0;
					int s = 0;
					bool pomCin = true;
					char *varDouble;
					if (!cinBool){
						c = getchar();
					}
					while (isspace(c)){
						c = getchar();
					}
					varDouble = newMalloc(sizeof(char)*2);
					if (error != ERR_OK) return error;
					varDouble[i] = '\0';
					while (pomCin){	
						switch (s){
							case 0: //pocatek
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 1;
								} else return ERR_VSTUP;
								break;
							case 1: //cislo
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else if (c == '.'){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 2;
								} else if ((c=='e') || (c=='E')){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 4;
								} else pomCin = false;
								break;
							case 2: //tecka
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 3;
								} else return ERR_VSTUP;
								break;
							case 3: //po tecce a cisle
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else if ((c=='e') || (c=='E')){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 4;
								} else pomCin = false;
								break;
							case 4: //e
           						if ((c=='+') || (c=='-')){
           							i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 5;
           						} else if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 6;
								} else return ERR_VSTUP;
								break;
							case 5: //+ a -
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 6;
								} else return ERR_VSTUP;
								break;
							case 6: //konec
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									if (error != ERR_OK) return error;
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else pomCin = false;
								break;
						}
					}
					if (varDouble[0] == '\0') return ERR_VSTUP;
					sscanf(varDouble, "%lf", &pom->data->hodnota->d);
					cinBool = true;
					pom->data->def = true;
				} else if (pom->data->typ == 3){
					int i = 0;
					bool pomCin = true;
					if (!cinBool){
						c = getchar();
					}
					while (isspace(c)){
						c = getchar();
					}
					pom->data->hodnota->s = newMalloc(sizeof(char)*2);
					if (error != ERR_OK) return error;
					pom->data->hodnota->s[i] = '\0';
					while (pomCin){	

						if (c == EOF || isspace(c)){
							pomCin = false;
						} else {
							i++;
							pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
							if (error != ERR_OK) return error;
							pom->data->hodnota->s[i-1] = c;
							pom->data->hodnota->s[i] = '\0';  
							c = getchar();
						}
						
					}

					pom->data->def = true;
					cinBool = true;
				}

				if (error != ERR_OK) return error;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;
		}
	}
	return error;
}
