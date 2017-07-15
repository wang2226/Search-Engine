
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
  maxNumber = 500;
  currentNumber = 0;
  array = new ArrayDictionaryNode[maxNumber];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
    // Add your code here
	// need to extend 
	if(currentNumber == maxNumber){
		int oldSize = maxNumber;
		maxNumber = 2 * maxNumber;
		ArrayDictionaryNode * newArray = new ArrayDictionaryNode[maxNumber];

		for(int i = 0; i < oldSize; i++){
			newArray[i].key = (char *)malloc(100 * sizeof(char));
			strcpy(newArray[i].key, array[i].key);
			newArray[i].data = new DataType;
			newArray[i].data = (DataType *)array[i].data;
		}
		delete [] array;
		array = newArray;
	}

	//finding key
	for(int i = 0; i < currentNumber; i++){
		if(strcmp(array[i].key,key) == 0){	//key found
			array[i].data = (DataType *)record;
			return false;
		}
	}
	
	//key not found, need to add
	array[currentNumber].key = (char *)malloc(100 * sizeof(char));
	strcpy(array[currentNumber].key,key);
	array[currentNumber].data = new DataType;
	array[currentNumber].data = (DataType *)record;
	currentNumber++;
	
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
    // add your code here
	for(int i = 0; i < currentNumber; i++){
		if(strcmp(array[i].key,key) == 0)
			return (DataType)array[i].data;
	}

	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
   	// Add your code here
	//finding key
	int found = 0;
	for(int i = 0; i < currentNumber; i++){
		if(strcmp(array[i].key,key) == 0)
			found = 1; //key found
	}

	if(found == 0)
		return false;

	ArrayDictionaryNode * newArray = new ArrayDictionaryNode[maxNumber];
	int index = 0;
	for(int i = 0; i < currentNumber; i++){
		if(strcmp(array[i].key,key) == 0){
			continue; //break the iteration
		}
		//copy
		newArray[index].key = (char *)malloc(100 * sizeof(char));
		strcpy(newArray[index].key,array[i].key);
		newArray[index].data = new DataType;
		newArray[index].data = (DataType *)array[i].data;

		index++;
	}

	delete [] array;
	array = newArray;
	currentNumber--;	

	return true;
	
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
    // Add yoru code here
	KeyType * keyArray = new KeyType[currentNumber];

	for(int i = 0; i < currentNumber; i++){
		keyArray[i] = (char *)malloc(100 * sizeof(char));
		strcpy((char *)keyArray[i], array[i].key);
	}
	*n = currentNumber;

	return keyArray;
}
