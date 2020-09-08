//Classes declaration and implentation header file

#ifndef _CLASSES_H
#define _CLASSES_H

#include <iostream.h>
#include <stdlib.h>
#include <time.h>

#define ORDER 30 //Use for page size = 256b 

//#define ORDER 5 //Use for debugging

class Node
{
private:
	int keyNo; //Key counter
	int keys[ORDER]; //Key array with size=ORDER-1 + 1 for O/F
	Node *children[ORDER+1]; //Children array with size=ORDER + 1 for O/F
	Node *father; //Father Node Pointer

public:
	Node(); //Constructor
	void putKey(int, int); //Puts key in keys array at the given index
	int getKey(int); //Returns key from keys array with given index
	Node *getChild(int); //Returns child from children array with given index
	void putChild(Node *, int); //Puts child in children array at the given index
	int insertKey(int); //Inserts a key at the keys array keeping the array sorted, returns the index of the inserted key
	void setkeyNo(int); //Sets the key counter
	int getkeyNo(); //Returns the key counter
	void setFather(Node *); //Sets Father node
	Node* getFather(); //Returns father node
	Node *split(int *); //Splits current node into 2 nodes
	int searchKeyRecursion(int, Node **, int *); //Searches for given key in node and in its children recursively
	void insertRecursion(Node *, Node *, int); //Implementation of the recursive B-Tree insertion algorithm
	int BinarySearchKeyRecursion(int, Node **, int *); //Searches for given key in node and in its children recursively (used for Binary Search)
	int BinaryRecursion(int, int, int); //Performs a Binary search on the keys array of the node for the given key
};

class Tree
{
private: 
	Node *root; //root pointer

public:
	Tree(); //Constructor
	Node* searchKey(int , int*, int *); //Searches for a given key, returns the pointer to the node where it was found 
										//and changes by reference the index it was found in and the memory accesses counter
	void insert(int); //Inserts key in the tree
	void setRoot(Node*); //Sets the tree root
	Node* getRoot(); //Returns the tree root
	Node *insertFindNodeRecursion(Node *, int); //Finds the node in which a keys is supposed to be inserted (recursively)
	Node* BinarySearchKey(int , int*, int *); //Searches for a given key. Same as searchKey(), but uses Binary search
};

Node::Node() //Constructor
{
	//Initialize all variables at "impossible" values
	int i=0;
	
	keyNo=-1;

	for(i=0; i<ORDER; i++)
		keys[i]=-1;

	for(i=0; i<ORDER+1; i++)
		children[i]=NULL;

	father=NULL;
}

void Node::setkeyNo(int value) //Sets the key counter
{
	keyNo = value;
}

int Node::getkeyNo() //Returns the key counter
{
	return keyNo;
}

void Node::setFather(Node* value) //Sets Father node
{
	father=value;
}

Node* Node::getFather() //Returns father node
{
	return father;
}

Tree::Tree() //Constructor
{
	//Initialize all variables at "impossible" values
	root=NULL;
}

Node* Tree::getRoot() //Returns the tree root
{
	return root;
}

void Tree::setRoot(Node* value) //Sets the three root
{
	root = value;
}

void Node::putKey(int value, int index) //Puts key in keys array at the given index
{
	keys[index]=value;
}

void Node::putChild(Node *value, int index) //Puts child in children array at the given index
{
	children[index]=value;
}

int Node::getKey(int index) //Returns key from keys array with given index
{
	return keys[index];
}


Node *Node::getChild(int index) //Returns child from children array with given index
{
	return children[index];
}

int Node::insertKey(int key) //Inserts a key at the keys array keeping the array sorted, returns the index of the inserted key
{
	if(keys[0]==-1) //If the array is empty
	{
		keys[0]=key; //Insert the key at the first position
		keyNo = 1; //set the counter at 1
		return 0; //return the index, in this case 0
	}

	//else insert the key keeping the array sorted

	for(int i=0; i<ORDER-1; i++) //run through the keys array
	{
		if(key<keys[i]) //if the key to be inserted is smaller than the current key in the array
		{
			for(int j=ORDER-1; j>i; j--) //Shift all keys one place to the right
				keys[j]=keys[j-1];
			keys[i]=key; //Insert key at the current position
			keyNo++; //Increment the key counter
			return i; //Return the index of the inserted key
		}
		else if(key>keys[i] && keys[i+1]==-1) //If alla the keys have been checked and the key to be inserted is larger than all of them
		{
			keys[i+1] = key; //insert key at the last position of the array
			keyNo++; //Increment the key counter
		    return i+1; //return the index of the inserted key
		}
	}
	return -1; //impossible control path, command used to avoid compiler warnings
}


Node* Tree::searchKey(int key, int *index, int *acceses)  //Searches for a given key, returns the pointer to the node where
						//it was found and changes by reference the index it was found in and the memory accesses counter
{
	Node *p=root; //initializes the pointer of the node the key was found in -to be changed by reference- at the root of the tree

	*index=root->searchKeyRecursion(key, &p, acceses); //The recursive search function is called at the root of the tree

	if(*index==-1) // if the index that was returned was -1, the key was not found
		return NULL; // return NULL

	return p; //else return the pointer to the node the key was found in
}

int Node::searchKeyRecursion(int key, Node **ptr, int *acceses) //Searches for given key in node and in its children recursively
{

	*acceses=*acceses+1; //Each time the function is called we have a new memory access, and the counter must be incremented
	int index=-1;  //Initialize the index at an -1

	*ptr=this; //Change the pointer to the node the key was found to the address of the current

	if(keys[0]==-1) //If the node is empty, the key is not here, so return index (still at -1)
		return index;

	int i;

	for(i=0; i<ORDER-1; i++) //Run through the keys array of the node
	{
		if(key==keys[i]) //If the key is found in the current node
			return i; //return the index of the key (definitely != -1)
	}

	//else...

	if(children[0]==NULL) //if the node is a leaf, the key is not in the tree, so return the index (still at -1)
		return index;

	//else...

	for(i=0; i<ORDER-1; i++) //Run again through the keys array
	{
		if(key<keys[i]) //Until the key searched is smaller than the current key
		{
			index=children[i]->searchKeyRecursion(key, ptr, acceses); //then search for the key in the left sub-tree
			break; //break the for loop thus ending the recursion
		}
		//else...
		if(keys[i+1]==-1) //AND ONLY if the next key is -1 which means the current key is the last in the array
		{
			index=children[i+1]->searchKeyRecursion(key, ptr, acceses); //search for the key in the right sub-tree
			break; //break the for loop thus ending the recursion
		}
	}
	
	return index; //return the index (unknown value, if the key was not found it is at -1)
}

Node *Tree::insertFindNodeRecursion(Node *start, int key) //Finds the node in which a keys is supposed to be inserted (recursively)
{

	if(start->getChild(0)==NULL) //if the node is a leaf, then this is where the key must be inserted
		return start; //So return its address

	for(int i=0; i<ORDER-1; i++) //Run through the keys array
	{
		if(key<start->getKey(i)) //If the key searched is smaller than the current key
			return insertFindNodeRecursion(start->getChild(i), key); //search the left sub-tree
		if(start->getKey(i+1)==-1) //AND ONLY if the next key is -1 which means the current key is the last in the array
			return insertFindNodeRecursion(start->getChild(i+1), key); //search the right sub-tree
	}
	return NULL; //Impossible control path; Command used to avoid compiler warnings
}

void Tree::insert(int key) //Inserts key in the tree
{
	int index;
	int feux; //Foo variable, used to complete parameters in searchKey function (normally used to count memory access times, not needed now)

	Node *p; // The node in which the key is supposed to be inserted

	p=searchKey(key, &index, &feux); //Search for the key to see if it already is in the tree

	if(p!=NULL) //if it is, the return value and the value of p are NULL
		return; //So the function returns

	//else...

	p=insertFindNodeRecursion(root, key); //The address of the node in which the key is supposed to be inserted is given by
											//the insertFindNodeRecursion

	if(p==NULL) //Check for NULL value from the impossible control path in insertFindNodeRecursion
	{
		cout<<"error";
		exit(1);
	}

	p->insertRecursion(NULL, NULL, key); //Call the insertion function for the node with initial parameters NULL, NULL and key

	return; //return after the key is inserted

}

void Node::insertRecursion(Node *left, Node *right, int middle) //Implementation of the recursive B-Tree insertion algorithm
{
	/*
	This function implements the recursive B-Tree insertion algorithm; It accepts 3 parameters: An integer (middle) 2 node pointers
	(left and right) which are the left and right children of the middle key, in case there was a split and the middle key was
	forwarded up to the father node to be inserted there.
	*/
	int index = insertKey(middle); //The key is inserted into the keys array, and the position where it was inserted is returned
	
	/*
	The children of the node now must be set accordingly, since the keys have changed places
	The index of the inserted key is known, its left and right children are also known
	thus enabling the re-assigning of the children pointers correctly. No discrimination
	between normal nodes and leafs is made, which means that NULL pointers to children will
	be treated as normal pointers.
	*/

	//Shift all children from the end of the children array to the place where the new children will be inserted by 1 place
	for(int i=ORDER; i>index+1; i--)
		children[i] = children[i-1];

	//And insert the 2 new children in the appropriate places
	children[index] = left;
	children[index+1] = right;

	/*
	Now the key has been inserted in the correct node and correct place, and check for overflow and further proccessing are made
	*/


	if(keyNo==ORDER) //If the key counter is equal to the Tree order, the node is in overflow state, and must be split
	{
		if(father==NULL) //If the father pointer is NULL, the node is the root
		{
			/*
			Because this function does not have access to the Tree's root pointer, instead of creating a new root node
			a new node is created with all the data from the original node, and the original node is blanked, as if
			the constructor was called. The blank node is set to be the father of the new node, and the split takes
			place in the new node instead.
			*/

			Node *newLeft = new Node(); //The new node is created

			//The data from the original node is copied to the new node

			for(int k=0; k<ORDER; k++)
				newLeft->putKey(keys[k], k);

			for(int l=0; l<ORDER+1; l++)
				newLeft->putChild(children[l], l);

			newLeft->setkeyNo(keyNo);

			//The original node is set as the father of the new node
			newLeft->setFather(this);

			//The original node is blanked

			keyNo=-1;
			for(i=0;i<(ORDER);i++)
				keys[i]=-1;
			for(i=0;i<ORDER+1;i++)
				children[i]=NULL;
			father=NULL;

			//The split takes place in the new node, which is now the left node of the 2 nodes that came from the split
			//and the right node is given by the split function.

			right=newLeft->split(&middle);


			//Now that the nodes have been split, their children need to have their father pointers
			//updated since they have changed

			for(int j=0; j<ORDER+1; j++)
			{
				if(newLeft->getChild(j)!=NULL)
					newLeft->getChild(j)->setFather(newLeft);
				if(right->getChild(j)!=NULL)
					right->getChild(j)->setFather(right);
			}

			//The middle key is forwarded up to the father, which is actually the current node
			insertRecursion(newLeft, right, middle);
			
			return; //return after inserting the key
		}
		else //If the father pointer is not NULL, then we are not at the root
		{
			
			right=split(&middle); //The current node is split, and becomes the left node of the 2 nodes that came from the split
									//and the right node is given by the split function
			
			//Now that the nodes have been split, their children need to have their father pointers
			//updated since they have changed

			for(int j=0; j<ORDER+1; j++)
			{
				if(children[j]!=NULL)
					children[j]->setFather(this);
				if(right->getChild(j)!=NULL)
					right->getChild(j)->setFather(right);
			}

			//The middle key is forwarded up to the father of the current node

			father->insertRecursion(this, right, middle);
			
			return; //Return after inserting the key
		}
	}
	else //We have no overflow, so we are done
		return;
}

Node *Node ::split(int *middle) //Splits current node into 2 nodes
{
	/*
	This function splits the current node into 2 nodes.
	The current node's data is all copied in temporary local variables, and the node is then blanked just as if the constructor
	was called. It will become the left node of the split. A new node is created to become the right node of the split
	and it's address is returned by the function. The middle key is changed by reference.
	*/

	int m = 0;

	Node *right = new Node(); //Create new node to become the right node of the split

	//Set of temporary local variables to store the data of the original node
	int arrayKeys[ORDER];
	Node *tempFather = father;
	Node *arrayChildren[ORDER+1];
	int tempKeyNo = keyNo;

	//The data from the original node is copied in the temp variables
	for(int i=0; i<ORDER; i++)
		arrayKeys[i] = keys[i];
	for(i=0; i<ORDER+1; i++)
		arrayChildren[i] = children[i];

	
	//The original node is blanked
	keyNo=-1;

	for(i=0;i<(ORDER);i++)
		keys[i]=-1;

	for(i=0;i<ORDER+1;i++)
		children[i]=NULL;

	father=NULL;


	//The father of the original node is now set as the father of the 2 new nodes
	father = tempFather;
	right->setFather(father);

	//The spliting of the data takes place

	if((ORDER % 2) == 0) //If the ORDER of the tree is even
	{
		for(int i=0; i<ORDER/2; i++) //Copy the appropriate keys to the left node
			insertKey(arrayKeys[i]);

		for(int j=0; j<(ORDER/2)+1; j++) //Copy the appropriate children to the left node
			children[j] = arrayChildren[j];

		for( i=(ORDER/2)+1; i<ORDER; i++) //Copy the appropriate keys to the right node
			right->insertKey(arrayKeys[i]);

		for(j=(ORDER/2)+1, m=0; j<ORDER+1; j++,m++) //Copy the appropriate children to the right node
			right->putChild(arrayChildren[j],m);

		*middle = arrayKeys[ORDER/2]; //Set the middle key accordingly
	}
	else //If the ORDER of the tree is odd
	{
		for(int i=0; i<(ORDER-1)/2; i++) //Copy the appropriate keys to the left node
			insertKey(arrayKeys[i]);

		for(int j=0; j<(ORDER+1)/2; j++) //Copy the appropriate children to the left node
			children[j] = arrayChildren[j];

		for( i=(ORDER+1)/2; i<ORDER; i++) //Copy the appropriate keys to the right node
			right->insertKey(arrayKeys[i]);

		for( j=(ORDER+1)/2, m=0; j<ORDER+1; j++, m++) //Copy the appropriate children to the right node
			right->putChild(arrayChildren[j],m);

		*middle = arrayKeys[(ORDER-1)/2]; //Set the middle key accordingly

	}

	return right; //return the address of the right node
}

Node* Tree::BinarySearchKey(int key, int *index, int *acceses) //Searches for a given key. Same as searchKey(), but uses Binary search
{
	Node *p=root; //initializes the pointer of the node the key was found in -to be changed by reference- at the root of the tree

	*index=root->BinarySearchKeyRecursion(key, &p, acceses); ///The recursive search function is called at the root of the tree

	if(*index==-1) //if the index that was returned was -1, the key was not found
		return NULL; //return NULL

	return p; //else return the pointer to the node the key was found in
}


int Node::BinarySearchKeyRecursion(int key, Node **ptr, int *acceses) //Searches for given key in node and in its children recursively
{

	*acceses=*acceses+1; //Each time the function is called we have a new memory access, and the counter must be incremented
	int index=-1;  //Initialize the index at an -1

	*ptr=this; //Change the pointer to the node the key was found to the address of the current

	if(keys[0]==-1) //If the node is empty, the key is not here, so return index (still at -1)
		return index;

	int i;

	index = BinaryRecursion(0, keyNo-1, key); //Performs the recusrive Binary search algorithm for the current node

	if(index != -1)
		return index;

	//else...

	if(children[0]==NULL) //if the node is a leaf, the key is not in the tree, so return the index (still at -1)
		return index;

	//else...

	for(i=0; i<ORDER-1; i++) //Run again through the keys array
	{
		if(key<keys[i]) //Until the key searched is smaller than the current key
		{
			index=children[i]->BinarySearchKeyRecursion(key, ptr, acceses); //then search for the key in the left sub-tree
			break; //break the for loop thus ending the recursion
		}
		//else...
		if(keys[i+1]==-1) //AND ONLY if the next key is -1 which means the current key is the last in the array
		{
			index=children[i+1]->BinarySearchKeyRecursion(key, ptr, acceses); //search for the key in the right sub-tree
			break; //break the for loop thus ending the recursion
		}
	}
	
	return index; //return the index (unknown value, if the key was not found it is at -1)
}

int Node::BinaryRecursion(int A, int B, int key) //Performs a Binary search on the keys array of the node for the given key
{
	if(A>B)			//if the low limit is larger than the right
		return -1;	//the function has exceeded its search limits, and returns.


	//This function uses 3 parameters, the rightmost and leftmost index of the (sub)array to be examined and the key searched

	int M;

	if(B-A == 0) //If the two parameters are equal then the subarray has only one element
	{
		if(key != keys[A]) //If the key of the array isn't equal to the key searched, return -1
			return -1;
		else //Else return the key's index
			return A;
	}

	//else, if the (sub)array has more than one elements

	if((B-A)%2 == 0) //If it's size is odd (B-A is even)
	{
		M = (B-A)/2+A; //set the middle index accordingly

		if(keys[M] == key) //if the middle key is equal to the searched key, return it's index
			return M;

		if(keys[M] > key) //If the middle key is larger, call the function for the left sub-array
			return BinaryRecursion(A, M-1, key);
		
		if(keys[M] < key) //If the middle key is smaller, call the function for the right sub-array
			return BinaryRecursion(M+1, B, key);
	}
	else //if it's size is even (B-A is odd)
	{
		M = ((B-A+1)/2)+A; //set the middle index accordingly

		if(keys[M] == key) //if the middle key is equal to the searched key, return it's index
			return M;

		if(keys[M] > key) //If the middle key is larger, call the function for the left sub-array
			return BinaryRecursion(A, M-1, key);

		if(keys[M] < key) //If the middle key is smaller, call the function for the right sub-array
			return BinaryRecursion(M+1, B, key);
	}

	return -1; //Impossible control path; Command used to avoid compiler warnings.
}


#endif
