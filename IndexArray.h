#pragma once
#include <stdlib.h>
#include <stdio.h>

#ifndef IX_ARRAY_H
#define IX_ARRAY_H

typedef struct {
	long* _headSort;
	long* _headRand;
	long _capacity;
	long _count;
	long _current;
} IndexArray;

IndexArray IndexArray_Create(long capacity, int fill);

int IndexArray_Insert(IndexArray* array, long index);
int IndexArray_Remove(IndexArray* array, long index);
int IndexArray_Find(IndexArray* array, long index);
int IndexArray_Randomize(IndexArray* array);
int IndexArray_Advance(IndexArray* array);
int IndexArray_Reverse(IndexArray* array);
int IndexArray_Current(IndexArray* array, int rand);

#endif // !IX_ARRAY_H