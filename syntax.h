#ifndef SYNTAX_H_HLAVICKA
#define SYNTAX_H_HLAVICKA

#include <ctype.h>               
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scaner.h"   
#include "alokator.h"
#include "data.h"
#include "random.h"
#include "vyrazy.h"

extern tTabulka *halda;
extern tGData *aktFunkce;
extern tInstrList pomPaska;

tError s_syntax();
tError s_funkce();
tError s_stat();
tError s_else(char *pomIE);
tError s_exp();
tError s_term();
tError s_param();
tError s_param2();
tError s_arg();
tError s_arg2();
tError s_cin();
tError s_cout();
tError s_cin2();
tError s_cout2();

char *newFrame(tTabulka* halda, tTabulka *newItem, int ramec);
char *newArgs(tTabulka* halda, tParamElemPtr newItem);
char *newLabel(tTabulka* halda);

#endif /* SYNTAX_H_HLAVICKA */