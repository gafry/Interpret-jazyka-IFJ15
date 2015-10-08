#include <stdio.h>
#include <stdlib.h>
#include "alokator.h"

#define ERR_ALLOC 5;

ptrAllocStruct firstStruct;
ptrAllocStruct lastStruct;

void *newMalloc(int size)
{
	ptrAllocStruct record = malloc(sizeof(struct AllocStruct));
	if (record == NULL)	{
		error = ERR_ALLOC;
		return NULL;
	}

	void *dataAlloc = (void*) malloc(size);
	if (dataAlloc == NULL) {
		error = ERR_ALLOC;
		return NULL;
	}

	record->data = dataAlloc;
	record->size = size;
	record->nextStruct = NULL;

	if (firstStruct == NULL) {
		firstStruct = record;
	}

	if (lastStruct == NULL)	{
		lastStruct = record;
	} else {
		record->nextStruct = lastStruct;
		lastStruct = record;
	}

	return dataAlloc;
}

void *newRealloc(void *adr, int size)
{
	ptrAllocStruct record = NULL;
	if (record == NULL) {
		adr = newMalloc(size);
		return adr;
	} else {
		record = realloc(adr, size);
		if (record != adr) {
			diffMem(record, size, adr);
		}
		return record;
	}
}

void diffMem(void *record, int size, void *toBeDestroyed) 
{
	seekAndDestroy(toBeDestroyed);

	ptrAllocStruct newRecord = malloc(sizeof(tAllocStruct));
	if (newRecord == NULL) {
		error = ERR_ALLOC;
		return;
	}

	newRecord->data = record;
	newRecord->size = size;
	newRecord->nextStruct = NULL;

	if (firstStruct == NULL) {
		firstStruct = newRecord;
	}

	if (lastStruct == NULL)	{
		lastStruct = newRecord;
	} else {
		newRecord->nextStruct = lastStruct;
		lastStruct = newRecord;
	}
}

void seekAndDestroy(void *toBeDestroyed) 
{
	ptrAllocStruct record = firstStruct;
	if (record == NULL) {
		return;
	}

	while (record != NULL) {
		if (toBeDestroyed == record->data) {
			record->data = NULL;
		}
		record = record->nextStruct;
	}
}

void killThemAll() 
{
	ptrAllocStruct record = NULL;

	record = firstStruct;
	while (record != NULL) {
		if (record->data != NULL) {
			free(record->data);
		}
		record = record->nextStruct;
	}

	record = firstStruct;
	while (record != NULL) {
		firstStruct = record;
		record = record->nextStruct;
		free(firstStruct); 
	}
}
