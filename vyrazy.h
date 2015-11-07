#include <stdlib.h>
#include <string.h>

typedef struct tPrvek {
	int data;
	struct tPrvek *next;
} *tPrvekPtr;

typedef struct {
	tPrvekPtr vrchol;
} tZasobnik;

void ZInit(tZasobnik *zasobnik);
int ZEmpty(tZasobnik *zasobnik);
void ZPush(tZasobnik *zasobnik, int data);
void ZPop(tZasobnik *zasobnik);
void ZTop(tZasobnik *zasobnik, int *data);
void ZTopPop(tZasobnik *zasobnik, int *data);
void ZClear(tZasobnik *zasobnik);