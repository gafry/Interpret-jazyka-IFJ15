#include "alokator.h"

ptrAllocStruct firstStruct;
ptrAllocStruct lastStruct;

void *newMalloc(int size)
{
	ptrAllocStruct record = malloc(sizeof(struct AllocStruct));
	if (record == NULL)	{
		error = ERR_INTER;
		return NULL;
	}

	void *dataAlloc = (void*) malloc(size);
	if (dataAlloc == NULL) {
		error = ERR_INTER;
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
		lastStruct->nextStruct = record;
		lastStruct = record;
	}

	return dataAlloc;
}

void *newRealloc(void *adr, int size)
{
	void *record = NULL;
	if (adr == NULL) {
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

	ptrAllocStruct newRecord = malloc(sizeof(struct AllocStruct));
	if (newRecord == NULL) {
		error = ERR_INTER;
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
		lastStruct->nextStruct = newRecord;
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
