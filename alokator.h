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
	ERR_OK,				//0
	ERR_LEX,			//1
	ERR_SYN,			//2
	ERR_SEM_NEDEF,		//3
	ERR_SEM_VYRAZ,		//4
	ERR_SEM_TYP,		//5
	ERR_SEM_OST,		//6
	ERR_VSTUP,			//7
	ERR_NEINIT,			//8
	ERR_NULA,			//9
	ERR_OST,			//10
	ERR_INTER = 99		//99
} tError;

extern tError error;

void *newMalloc(int size);
void *newRealloc(void *adr, int size);
void diffMem(void *record, int size, void *toBeDestroyed);
void seekAndDestroy(void *toBeDestroyed);
void killThemAll();

#endif /* ALOKATOR_H_GARBAGE */