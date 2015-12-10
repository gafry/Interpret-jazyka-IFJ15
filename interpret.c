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

//jen pro vyrazy
int exp_counter = 0;

tError preloz(){

	ZInit(&ramce);
	ILInit(&globalniPaska);
	GCopyMain (&global, &globalniPaska);
	ILFirst(&globalniPaska);

	while (1){
//printf("\n cislo : %d\n", globalniPaska.Act->instr.op);
		switch (globalniPaska.Act->instr.op){

			case I_START:

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

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
				break;

			case I_PUSH_FRAME:

				pom = TRPSearch(halda, globalniPaska.Act->instr.adr1);
				ZPush(&ramce, pom->data->hodnota->tabulka, pom->data->ramec);

				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_POP_FRAME:

				ZPop(&ramce, 2);
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_POP_ALL:

				ZPop(&ramce, 1);
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_COUT:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL) return ERR_SEM_NEDEF;

				if (pom->data->typ == 1 && pom->data->def){ //printf("%s....%d\n", pom->data->nazev, pom->data->hodnota->i);
					printf("%d", pom->data->hodnota->i);
				} else if (pom->data->typ == 2 && pom->data->def){  //printf("%s....%g\n", pom->data->nazev, pom->data->hodnota->d);
					printf("%g", pom->data->hodnota->d); //printf("%s....%g\n", pom->data->nazev, pom->data->hodnota->d);
				} else if (pom->data->typ == 3 && pom->data->def){
					printf("%s", pom->data->hodnota->s);					
				} else return ERR_SEM_NEDEF;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

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
							}						
						}
					}
				}
				break;

			case I_ELSE:
				
				if (globalniPaska.Act->jump != NULL){
					globalniPaska.Act = globalniPaska.Act->jump;
				} else globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_IFELSE:
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_RETURN:

				pomReturn = true;
				pomBool = true;
				pomName = globalniPaska.Act->instr.adr1;
				globalniPaska.Pom = globalniPaska.Act->lptr;
				//pom = ZSearch(&ramce, globalniPaska.Pom->instr.vysl);
				while (pomBool){
					globalniPaska.Act = globalniPaska.Act->rptr;
					if (globalniPaska.Act->instr.op == I_KONEC || globalniPaska.Act->instr.op == I_KONEC_FUNKCE){
						if (globalniPaska.Act->instr.adr1 == pomName){
							pomBool = false;
						}						
					}
				}
				if (globalniPaska.Act->instr.op != I_KONEC){
					globalniPaska.Act->rptr->rptr->instr.adr1 = globalniPaska.Pom->instr.vysl;
				}
				break;

			case I_FUNKCE:

				/* V casti 1 zkontroluju, zda sedi typy parametru a argumentu, pokud je argument
				   identifikator, tak ho najdu a pak az zkontroluju typ.
				   V casti 2 nakopiruju instrukce dane funkce do globalni pasky na aktualni pozici
				   V casti 3 predam nazev funkce a odkaz na argumenty funkce instrukci I_START
				   V casti 4 prejdu na dalsi instrukci */

				//CAST 1
				pomData = GFindData (&global, globalniPaska.Act->instr.adr1); 
				parametry = pomData->params;
				pom = TRPSearch(halda, globalniPaska.Act->instr.adr2);
				argumenty = pom->data->hodnota->args;
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
						if (pom2->data->typ != parametry->data->typ) return ERR_SEM_VYRAZ;
					} else if (parametry->data->typ != argumenty->data->typ) return ERR_SEM_VYRAZ;
					parametry = parametry->rptr;
					argumenty = argumenty->rptr;
				}

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
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_KONEC:
				if (pomReturn == false){
					return ERR_NEINIT;
				}
				return error;
				break;

			case I_PRIR:
//printf("\n%s ... %s\n", globalniPaska.Act->instr.adr1, globalniPaska.Act->instr.vysl);
			
                pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				} 
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF; 
				}

                if ((pom->data->typ == 1) && (pom3->data->typ == 1)){
                	pom3->data->hodnota->i = pom->data->hodnota->i;
                    pom3->data->typ = 1; 
                } else if ((pom->data->typ == 1) && (pom3->data->typ == 2)){                       
                    pom3->data->hodnota->d = (double) pom->data->hodnota->i;
                    pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom3->data->typ == 1)){
                	pom3->data->hodnota->i = (int) pom->data->hodnota->d;
                    pom3->data->typ = 1;
                } else if ((pom->data->typ == 2) && (pom3->data->typ == 2)){
                	pom3->data->hodnota->d = pom->data->hodnota->d ;
                    pom3->data->typ = 2;
                } else if (pom->data->typ == 3){                       
                    pom3->data->hodnota->s = pom->data->hodnota->s ;
                    pom3->data->typ = 3;
                } else return ERR_SEM_VYRAZ;
                pom3->data->def = true; 
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_EXP_LAST:

				exp_counter++;
				if (exp_counter < 10){
					expCheck = 1;
				} else expCheck = 0;
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_ADD:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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

                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_SUB:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_MUL:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_DIV:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

                if ((pom->data->typ == 1) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->i = pom->data->hodnota->i / pom2->data->hodnota->i;
               		pom3->data->typ = 1;
                } else if ((pom->data->typ == 1) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->i / (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 1) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d / (double) pom2->data->hodnota->i;
                	pom3->data->typ = 2;
                } else if ((pom->data->typ == 2) && (pom2->data->typ == 2) && (pom3->data->typ != 3)){
                	if (pom2->data->hodnota->i == 0) return ERR_NULA;
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	pom3->data->hodnota->d = (double) pom->data->hodnota->d / (double) pom2->data->hodnota->d;
                	pom3->data->typ = 2;
                } else {
                	return ERR_SEM_VYRAZ;
                }
                globalniPaska.Act = globalniPaska.Act->rptr;
                break;

			case I_VET:

                pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s > pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_MEN:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
//printf("%s ... %d\n", pom->data->nazev, pom->data->hodnota->i);
//printf("%s ... %d\n", pom2->data->nazev, pom2->data->hodnota->i);
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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s < pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_VETROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s >= pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_MENROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s <= pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                
			case I_ROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s == pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

			case I_NEROV:
				
				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom2 = ZSearch(&ramce, globalniPaska.Act->instr.adr2);
                if (pom2 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}
                pom3 = ZSearch(&ramce, globalniPaska.Act->instr.vysl);
                if (pom3 == NULL || pom->data->def == false){
					return ERR_SEM_NEDEF;
				}

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
                } else if ((pom->data->typ == 3) && (pom2->data->typ == 3)){
                	pom3->data->hodnota = newMalloc(sizeof(tHodnota));
                	if (pom->data->hodnota->s != pom2->data->hodnota->s){                            
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
                } else return ERR_SEM_VYRAZ;
                globalniPaska.Act = globalniPaska.Act->rptr;
				break;

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
					varInt[i] = '\0';
					while (pomCin){		
						if (isspace(c) || c == EOF){
							if (varInt[0] == '\0'){
								return ERR_VSTUP;
							}
							pomCin = false;
						} else if (isdigit(c)){
							i++;
							varInt = newRealloc(varInt, (sizeof(char)));
							varInt[i-1] = (char) c;
							varInt[i] = '\0';//printf("\nprom ... %s ... %d\n", pom->data->hodnota->s, c);
							c = getchar();
						} else {
							return ERR_VSTUP;
						}
					}
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
					varDouble[i] = '\0';
					while (pomCin){	
						switch (s){
							case 0: //pocatek
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
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
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else if (c == '.'){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 2;
								} else if ((c=='e') || (c=='E')){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 4;
								} else if (isspace(c) || c == EOF){
									pomCin = false;
								} else return ERR_VSTUP;
								break;
							case 2: //tecka
								if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
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
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else if ((c=='e') || (c=='E')){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 4;
								} else if (isspace(c) || c == EOF){
									pomCin = false;
								} else return ERR_VSTUP;
								break;
							case 4: //e
           						if ((c=='+') || (c=='-')){
           							i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
									s = 5;
           						} else if (isdigit(c)){
									i++;
									varDouble = newRealloc(varDouble, (sizeof(char)));
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
									varDouble[i-1] = (char) c;
									varDouble[i] = '\0';
									c = getchar();
								} else if (isspace(c) || c == EOF){
									pomCin = false;
								} else return ERR_VSTUP;
								break;
						}
					}
					sscanf(varDouble, "%lf", &pom->data->hodnota->d);
					cinBool = true;
					pom->data->def = true;
				} else if (pom->data->typ == 3){
					int i = 0;
					int stav = 0;
					int pomZnak = 0;
					int pomVypocet = 0;
					bool pomCin = true;
					if (!cinBool){
						c = getchar();
					}
					while (isspace(c)){
						c = getchar();
					}
					pom->data->hodnota->s = newMalloc(sizeof(char)*2);
					pom->data->hodnota->s[i] = '\0';
					while (pomCin){	

						switch (stav){
							case 0:{
					            if (c == '\\'){
    					            stav = 1;
    					            c = getchar();
        					    } else if (isspace(c) || c == EOF){
					                pomCin = false;
					            } else {  
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) c;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					            }
					            break;
					        }

					        case 1:{
					        	if (isspace(c) || c == EOF){
					        		pomCin = false;
					        	} else if (c == '\\'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 92;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
									stav = 0;
					            } else if (c == '"'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 34; 
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
									stav = 0;
					            } else if (c == 'n'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 10;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
									stav = 0;
					            } else if (c == 't'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 9;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
									stav = 0;
					            } else if (c == 'x'){
					                stav = 2;
					                c = getchar();
					            } else {
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) c;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					                stav = 0;
					            }
					            break;
					        }

					        case 2:{
					            if (isspace(c) || c == EOF){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 'x';
									pom->data->hodnota->s[i] = '\0';  
					                pomCin = false;
					            } else if (c == '\\'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) 'x';
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					                stav = 1;
					            } else if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F'))){
					                pomZnak = c;
					                c = getchar();
					                stav = 3;
					            } else {
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 'x';
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) c;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					                stav = 0;
					            }
					            break;
					        }

					        case 3:{
					            if (isspace(c) || c == EOF){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 'x';
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) pomZnak;
									pom->data->hodnota->s[i] = '\0';  
					                pomCin = false;
					            } else if (c == '\\'){
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 'x';
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ pomZnak;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					                stav = 1;
					            } else if (pomZnak == '0' && c == '0'){
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 92;
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ 'x';
									pom->data->hodnota->s[i] = '\0';  
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) pomZnak;
									pom->data->hodnota->s[i] = '\0';
									i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = (char) c;
									pom->data->hodnota->s[i] = '\0'; 
									c = getchar();
					                stav = 0;
					            } else if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F'))){
					                if (pomZnak == '0') pomVypocet = 0;
					                else if (pomZnak == '1') pomVypocet = 16;
					                else if (pomZnak == '2') pomVypocet = 32;
					                else if (pomZnak == '3') pomVypocet = 48;
					                else if (pomZnak == '4') pomVypocet = 64;
					                else if (pomZnak == '5') pomVypocet = 80;
					                else if (pomZnak == '6') pomVypocet = 96;
					                else if (pomZnak == '7') pomVypocet = 112;
					                else if (pomZnak == '8') pomVypocet = 128;
					                else if (pomZnak == '9') pomVypocet = 144;

					                if (c == '0') pomVypocet = pomVypocet + 0;
					                else if (c == '1') pomVypocet = pomVypocet + 1;
					                else if (c == '2') pomVypocet = pomVypocet + 2;
					                else if (c == '3') pomVypocet = pomVypocet + 3;
					                else if (c == '4') pomVypocet = pomVypocet + 4;
					                else if (c == '5') pomVypocet = pomVypocet + 5;
					                else if (c == '6') pomVypocet = pomVypocet + 6;
					                else if (c == '7') pomVypocet = pomVypocet + 7;
					                else if (c == '8') pomVypocet = pomVypocet + 8;
					                else if (c == '9') pomVypocet = pomVypocet + 9;
					                i++;
									pom->data->hodnota->s = newRealloc(pom->data->hodnota->s, (sizeof(char)));
									pom->data->hodnota->s[i-1] = /*(char)*/ pomVypocet;
									pom->data->hodnota->s[i] = '\0';  
									c = getchar();
					                pomVypocet = 0;
					                stav = 0;
					            }
					            break;
					        }
						}
					}

					pom->data->def = true;
					cinBool = true;
				}
				globalniPaska.Act = globalniPaska.Act->rptr;
				break;
		}
	}
	return error;
}
