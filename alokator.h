#ifndef ALOKATOR_H_GARBAGE
#define ALOKATOR_H_GARBAGE

#include <stdio.h>
#include <stdlib.h>

typedef struct AllocStruct
{
    void *data;
    int size;
    void *nextStruct;
} tAllocStruct, *ptrAllocStruct;

typedef enum {
	ERR_OK,
	ERR_LEX,
	ERR_SYN,
	ERR_SEM_NEDEF,
	ERR_SEM_VYRAZ,
	ERR_SEM_TYP,
	ERR_SEM_OST,
	ERR_VSTUP,
	ERR_NEINIT,
	ERR_NULA,
	ERR_OST,
	ERR_INTER = 99	
} tError;

extern tError error;

void *newMalloc(int size);
void *newRealloc(void *adr, int size);
void diffMem(void *record, int size, void *toBeDestroyed);
void seekAndDestroy(void *toBeDestroyed);
void killThemAll();

#endif /* ALOKATOR_H_GARBAGE */