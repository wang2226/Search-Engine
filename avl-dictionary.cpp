
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	// Add your implementation here

	//Find node to insert into 
	AVLNode * curr = root;
	AVLNode * prev = NULL;
	
	while(curr != NULL){
		prev = curr;
		if(strcmp(key,curr->key) == 0){
			//key found, substitute data
			curr->data = record;
			return false;
		} else if(strcmp(key,curr->key) < 0){
			curr = curr->left;
		} else {	//key > curr->key
			curr = curr->right;
		}
	} //while

	//Node does not exist. Create it.
	AVLNode * n = new AVLNode();
	n->key = key;
	n->data = record;
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;
	n->height = 1;

	//test if tree is empty
	if(prev == NULL){	//root is NULL
		//insert n as root node
		return false;
	}

	//tree is not empty, prev points to the parent where new node will be inserted 
	if(strcmp(key,prev->key) < 0){
		//insert left
		prev->left = n;
	} else {
		//insert right
		prev->right = n;
	}

	n->parent = prev;

	//Height might not be valid anymore.
	AVLNode * m = n->parent;
	while(m != NULL){	//iterate until root
		int maxheight = 0;
		if(m->left != NULL){	//check if left exists
			maxheight = m->left->height;
		} 
		if(m->right != NULL && m->right->height > maxheight){
			maxheight = m->right->height;
		}
		m->height = maxheight + 1;
		m = m->parent;	//go to the parent to compute height
	}	//while

	//We need to restructure .
	restructure(n);

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
		
	nElements++;
	return true;
}	//insert

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	//Go upwards until root is found

        // See class notes

	// Add your implementation here
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here

	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here
	
	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}
	
	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

