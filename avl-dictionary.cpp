
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
	if(root == NULL){	//root is NULL
		//insert n as root node
		root = n;
		nElements++;
		return true;
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

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure
	restructure(n);

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
	AVLNode * z = n->parent;	//z is the root of the unbalanced subtree
	
	while(z != NULL){
		//check if this z is the parent of unbalanced subtree
		int hleft = 0;
		if(z->left != NULL){
			hleft = z->left->height;
		}

		int hright = 0;
		if(z->right != NULL){
			hright = z->right->height;
		}

		int hdiff = hright - hleft;
		if(hdiff < 0) hdiff = -hdiff;	//obtain absolute val
		if(hdiff <= 1){
			//z is balanced
			//go up to find a z that is unbalanced
			z = z->parent;
			continue;
		}

		//z is unbalanced
		//find y (child of z with max height)
		AVLNode * y = NULL;
		int maxh = 0;

		if(z->left != NULL){
			y = z->left;	//test left
			maxh = z->left->height;
		}
		if(z->right != NULL && maxh < z->right->height){
			y = z->right;
		}

		//y found
		assert(y != NULL);

		//find x
		AVLNode * x = NULL;
		maxh = 0;
		if(y->left != NULL){
			x = y->left;
			maxh = y->left->height;
		}
		if(y->right != NULL && maxh < y->right->height){
			x = y->right;
		}

		//x found
		assert(x != NULL);

		//identify a,b,c,t0,t1,t2,t3
		AVLNode *a, *b, *c, *t0, *t1, *t2, *t3;

		if(z->right == y){
			if(y->right == x){
				//case 1
				a = z;
				b = y;
				c = x;
				t0 = z->left;
				t1 = y->left;
				t2 = x->left;
				t3 = x->right;
			} else {
				//case 2
				a = z;
				c = y;
				b = x;
				t0 = z->left;
				t1 = x->left;
				t2 = x->right;
				t3 = y->right;
			} 

		} else {
			//case 3 or 4
			if(y->left == x){
				//case 3
				a = x;
				b = y;
				c = z;
				t0 = x->left;
				t1 = x->right;
				t2 = y->right;
				t3 = z->right;
			} else {
				//case 4
				a = y;
				b = x;
				c = z;
				t0 = y->left;
				t1 = x->left;
				t2 = x->right;
				t3 = z->right;
			}
		}	//else
		
		//do rotation
		AVLNode * p = z->parent;

		if(p != NULL){
			//not root
			if(p->left == z){
				//attach b to left of p
				p->left = b;
			} else {
				//attach b to right of p
				p->right = b;
			}
		} else {
			//p == NULL, then z is root
			root = b;
		}

		//connect a,b,c to t0,t1,t2,t3
		b->parent = p;
		b->left = a;
		b->right = c;
		a->parent = b;
		a->left = t0;
		a->right = t1;
		c->parent = b;
		c->left = t2;
		c->right = t3;

		//connect parent of t0,t1,t2,t3
		if(t0 != NULL){
			t0->parent = a;
		}
		if(t1 != NULL){
			t1->parent = a;
		}
		if(t2 != NULL){
			t2->parent = c;
		}
		if(t3 != NULL){
			t3->parent = c;
		}

		//fix height of a
		int maxheight = 0;
		if(a->left != NULL){
			maxheight = a->left->height;
		}
		if(a->right != NULL && maxheight < a->right->height){
			maxheight = a->right->height;
		}
		a->height = maxheight + 1;

		//fix height of c
		maxheight = 0;
		if(c->left != NULL){
			maxheight = c->left->height;
		}
		if(c->right != NULL && maxheight < c->right->height){
			maxheight = c->right->height;
		}
		c->height = maxheight + 1;

		//fix height of b
		maxheight = 0;
		if(b->left != NULL){
			maxheight = b->left->height;
		}
		if(b->right != NULL && maxheight < b->right->height){
			maxheight = b->right->height;
		}
		b->height = maxheight + 1;

		//go up in the restructing
		z = z->parent;

	} //while

}//restructure

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
    // Add your implementation here
	AVLNode * p = root;

	while(p != NULL){
		if(strcmp(p->key,key) == 0){
			return (DataType)p->data;
		} else if(strcmp(p->key,key) > 0){
			p = p->left;
		} else {	//p->key < key
			p = p->right;
		}
	}

	return NULL;
}

//Find a key in the dictionary and return corresponding node or NULL
AVLNode *
AVLDictionary::findNode(KeyType key){
	AVLNode * p = root;

	while(p != NULL){
		if(strcmp(p->key,key) == 0){
			return p;
		} else if(strcmp(p->key,key) > 0){
			p = p->left;
		} else {	//p->key < key
			p = p->right;
		}
	}

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
	AVLNode * node;
	node = findNode(key);

	if(node == NULL){	//node to remove is empty
		return false;
	}
		
	if(node->left == NULL && node->right == NULL){	//node to remove has no children
		if(node->parent == NULL){
			delete node;
			return true;
		}
		if(node == node->parent->left){	//node to remove is its parent's left child
			node->parent->left = NULL;
		} else {	//node to remove is its parent's right child
			node->parent->right = NULL;
		}

		//adjust heights of nodes in path
		//from inserted node to root
		AVLNode * p;
		p = node->parent;

		while(p != NULL){	//iterate until root
			int maxheight = 0;

			if(p->left != NULL){	//check if left exists
				maxheight = p->left->height;
			}
			if(p->right != NULL && p->right->height > maxheight){
				maxheight = p->right->height;
			}
			p->height = maxheight + 1;
			p = p->parent;	//go to parent to recompute height
		} //while

		restructure(node->parent);
		delete node;
	} else if(node->left == NULL){	//node to remove only has right child
		//swap node to remove with its right child and delete node
		AVLNode temp;
		temp.height = node->height;
		temp.key = strdup((char *)node->key);
		temp.data = node->data;

		node->height = node->right->height;
		node->key = strdup((char *)node->right->key);
		node->data = node->right->data;

		node->right->height = temp.height;
		node->right->key = strdup((char *)temp.key);
		node->right->data = temp.data;

		delete node->right;
		node->right = NULL;

		//update height
		AVLNode * p = node->parent;
		while(p != NULL){
			int maxheight = 0;
			if(p->left != NULL){
				maxheight = p->left->height;
			}
			if(p->right != NULL && maxheight < p->right->height){
				maxheight = p->right->height;
			}
			
			p->height = maxheight + 1;
			p = p->parent;
		}	//while

		restructure(node);
	} else if(node->right == NULL){	//node to remove only has left child
		//swap node with its left child and delete node
		AVLNode temp;
		temp.height = node->height;
		temp.key = strdup((char *)node->key);
		temp.data = node->data;

		node->height = node->left->height;
		node->key = strdup((char *)node->left->key);
		node->data = node->left->data;

		node->left->height = temp.height;
		node->left->key = strdup((char *)temp.key);
		node->left->data = temp.data;

		delete node->left;
		node->left = NULL;

		//update height
		AVLNode * p = node->parent;
		while(p != NULL){
			int maxheight = 0;
			if(p->left != NULL){
				maxheight = p->left->height;
			}
			if(p->right != NULL && maxheight < p->right->height){
				maxheight = p->right->height;
			}
			
			p->height = maxheight + 1;
			p = p->parent;
		}	//while

		restructure(node);
	} else {	//node to remove has both left and right child
		AVLNode * p;
		p = node->left;

		if(p->right == NULL){	//obtain successor
			p = node->right;
			while(p->left != NULL){
				p = p->left;
			}
		} else {	//obtain predecessor
			while(p->right != NULL){
				p = p->right;
			}
		}
		
		//swap node with either successor or predecessor
		AVLNode temp;
		temp.height = node->height;
		temp.key = strdup((char *)node->key);
		temp.data = node->data;

		node->height = p->height;
		node->key = strdup((char *)p->key);
		node->data = p->data;

		p->height = temp.height;
		p->key = strdup((char *)temp.key);
		p->data = temp.data;

		//delete node
		AVLNode * n;
		n = p->parent;

		if(n != NULL){
			if(p == n->left){	//node is on the left side
				n->left = NULL;
				delete p;
			} else {	//node is on the right side
				n->right = NULL;
				delete p;
			}

			//update heights
			AVLNode * m = n;
			while(m != NULL){
				int maxheight = 0;
				if(m->left != NULL){
					maxheight = m->left->height;
				}
				if(m->right != NULL && maxheight < m->right->height){
					maxheight = m->right->height;
				}

				m->height = maxheight + 1;
				m = m->parent;
			}	//while

			restructure(n);
		}
	}

	nElements--;
	
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

