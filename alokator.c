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

#include "alokator.h"

ptrAllocStruct firstStruct;
ptrAllocStruct lastStruct;
tError error;

// alokuje pamet
void *newMalloc(int size)
{
	void *dataAlloc = NULL;
	dataAlloc = malloc(size);
	if (dataAlloc == NULL) {
		error = ERR_INTER;
		return NULL;
	}
	
	ptrAllocStruct record = NULL;
	record = malloc(sizeof(struct AllocStruct));
	if (record == NULL)	{
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

//realokuje pamet
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

// pomocna funkce
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

//pomocna funkce
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

//uvolni veskerou pamet
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