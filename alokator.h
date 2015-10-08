#pragma once

typedef struct AllocStruct
{
    void *data;
    int size;
    void *nextStruct;
} tAllocStruct, *ptrAllocStruct;

extern int error;

void *newMalloc(int size);
void *newRealloc(void *adr, int size);
void diffMem(void *record, int size, void *toBeDestroyed);
void seekAndDestroy(void *toBeDestroyed);
void killThemAll();
