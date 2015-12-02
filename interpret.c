#include "interpret.h"

tInstrList globalniPaska;
tGList global;
tError error;
tZasobnik ramce;
tTabulka *halda;
tTRPPolozka *pom;
int check = 0;

tError preloz(){

	ZInit(&ramce);
	ILInit(&globalniPaska);
	GCopyMain (&global, &globalniPaska);
	ILFirst(&globalniPaska);

	while (1){

		switch (globalniPaska.Act->instr.op){

			case I_START:
				break;

			case I_LABEL:
				break;

			case I_PUSH_FRAME:

				pom = TRPSearch(halda, globalniPaska.Act->instr.adr1);
				ZPush(&ramce, pom->data->hodnota->tabulka, pom->data->boss);

				break;

			case I_POP_FRAME:
				break;

			case I_POP_ALL:
				break;

			case I_CIN:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom->data->typ == 1){
					check = scanf("%d", &pom->data->hodnota->i);
					pom->data->def = true;
				} else if (pom->data->typ == 2){
					check = scanf("%lf", &pom->data->hodnota->d);
					pom->data->def = true;
				} else if (pom->data->typ == 3){

				}

				if (check != 1){
					return ERR_VSTUP;
				}

				break;

			case I_COUT:

				pom = ZSearch(&ramce, globalniPaska.Act->instr.adr1);
				if (pom->data->typ == 1 && pom->data->def){
					printf("%d", pom->data->hodnota->i);
				} else if (pom->data->typ == 2 && pom->data->def){
					printf("%g", pom->data->hodnota->d);
				} else if (pom->data->typ == 3 && pom->data->def){

				}
				break;

			case I_KONEC:
				return error;
				break;

			case I_ADD:
				break;

			case I_SUB:
				break;

			case I_MUL:
				break;

			case I_DIV:
				break;

			case I_PRIR:
				break;
		}

		globalniPaska.Act = globalniPaska.Act->rptr;
	}
	return error;
}