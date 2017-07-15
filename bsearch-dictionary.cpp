
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}
	
	// Use binary search

	// Add your implementation here
	int low = 0;
	int high = currentNumber-1;

	int mid = (high + low) / 2;
	while(low <= high){
		int flag = strcmp(key, array[mid].key);
		if(flag < 0){ //on the left side of mid
			high = mid - 1;
			mid = (low + high) / 2;
		} else if(flag > 0){ //on the right side of mid
			low = mid + 1;
			mid = (low + high) / 2;
		} else { //found
			return (DataType)array[mid].data;
		}
	}

	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	//printf("Before\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}

        // Add your code here

	//printf("After\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}
	
	//use bubble sort
	for(int i = 0; i < currentNumber - 1; i++){
		for(int j = 0; j < currentNumber - i - 1; j++){
			if(strcmp(array[j].key, array[j+1].key) > 0){ 
				//swap
				ArrayDictionaryNode temp;
				temp.key = (char *)malloc(100 * sizeof(char));
				temp.data = new DataType;

				temp.key = array[j].key;
				temp.data = array[j].data;

				array[j].key = array[j+1].key;
				array[j].data = array[j+1].data;

				array[j+1].key = temp.key;
				array[j+1].data = temp.data;
			}
		}
	}
}




