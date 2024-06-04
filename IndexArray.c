#include <IndexArray.h>

IndexArray IndexArray_Create(long capacity, int fill) {
	IndexArray newArray = {
		malloc(sizeof(long) * capacity),	// Sorted
		malloc(sizeof(long) * capacity),	// Random
		capacity,							// Capacity
		fill ? capacity : 0,				// Count
		0									// Start
	};

	if (fill) {
		for (int i = 0; i < capacity; i++)
			newArray._headSort[i] = i;

		IndexArray_Randomize(&newArray);
	}

	return newArray;
}

int IndexArray_Insert(IndexArray* array, long index)
{
	if (!array) {
		printf("Null array!\n");
		return 0;
	}

	if (array->_count == array->_capacity) {
		printf("Array max size! [%d]\n", array->_count);
		return 0;
	}
		
	if (index < 0 || index >= array->_capacity) {
		printf("Index out of array bounds!\n");
		return 0;
	}

	int previous = array->_headSort[0];
	int current, target;

	for (target = 0; target < array->_count; target++) {

		// Check each index for match
		current = array->_headSort[target];
		
		if (index == current) {
			printf("Index exists!\n");
			return 0;
		}

		// Check if the value goes between
		if (previous < index && current > index) {
			break;
		}

		previous = current;
	}

	// Iterate from last element to the target element
	for (int i = array->_count - 1; i >= target; i--)
		array->_headSort[i + 1] = array->_headSort[i];

	// Set and increment
	array->_headSort[target] = index;
	array->_count++;

	return IndexArray_Randomize(array);
}

int IndexArray_Remove(IndexArray* array, long index)
{
	if (!array) {
		printf("Null array!\n");
		return 0;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return 0;
	}

	if (index < 0 || index >= array->_capacity) {
		printf("Index out of array bounds!\n");
		return 0;
	}

	int current, target;

	for (target = 0; target < array->_count; target++) {

		// Check each index for match
		current = array->_headSort[target];

		if (index == current) {
			break;
		}
	}

	if (target == array->_count) {
		printf("Index doesn't exist!\n");
		return 0;
	}

	// Iterate from target element to the second last element
	for (int i = index; i < array->_count - 1; i++)
		array->_headSort[i] = array->_headSort[i + 1];

	array->_count--;

	return IndexArray_Randomize(array);
}

int IndexArray_Find(IndexArray* array, long index)
{
	if (!array) {
		printf("Null array!\n");
		return -1;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return -1;
	}

	for (int i = 0; i < array->_count; i++)
		if (array->_headRand[i] == index)
			return i;

	return -1;
}

int IndexArray_Randomize(IndexArray* array)
{
	if (!array) {
		printf("Null array!\n");
		return 0;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return 0;
	}

	int index;

	for (int i = 0; i < array->_count; i++)
		array->_headRand[i] = -1; // Clear Random Bin

	for (int i = 0; i < array->_count; i++) {
		index = rand(); // Generate random index
		index %= array->_count; // Bound within the array range

		while (IndexArray_Find(array, index) > -1) { // Look for match
			index++; // Increment and try again
			index = index >= array->_count ? 0 : index; // Roll-around to beginning
		}
		array->_headRand[i] = index;
		//printf("RandInt: %d | %d\n", i, index);
	}

	return 1;
}

int IndexArray_Advance(IndexArray* array)
{
	if (!array) {
		printf("Null array!\n");
		return 0;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return 0;
	}

	array->_current++;

	if (array->_current >= array->_count) {
		array->_current = 0;
		IndexArray_Randomize(array);
	}

	return 1;
}

int IndexArray_Reverse(IndexArray* array)
{
	if (!array) {
		printf("Null array!\n");
		return 0;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return 0;
	}

	array->_current--;

	if (array->_current < 0) {
		array->_current = array->_count - 1;
		IndexArray_Randomize(array);
	}

	return 1;
}

int IndexArray_Current(IndexArray* array, int rand)
{
	if (!array) {
		printf("Null array!\n");
		return -1;
	}

	if (array->_count == 0) {
		printf("Array empty!\n");
		return -1;
	}

	int c = array->_current;
	return rand ? array->_headRand[c] : array->_headSort[c];
}


