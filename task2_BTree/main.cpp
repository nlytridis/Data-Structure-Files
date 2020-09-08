#include "classes.h"

void main()
{
	
	Tree *tree = new Tree(); //Create a new empty tree
	Node *root = new Node(); //Create a new empty node to be the root
	
	tree->setRoot(root); //Set the root to be the tree root

	int i, j;
	int m=1000000; //Number of keys inserted in the tree
	int accesses=0, binAccesses=0, index=-1;

	Node *check; //Node where the key was found. If it wasnt found this equals NULL

	srand( (unsigned)time( NULL ) ); //Initialize rand();

	//Insert m random keys in tree
	cout<<"Inserting "<<m<<" random keys in tree... (Order = "<<ORDER<<")"<<endl;
	for(i=0; i<m; i++)
		tree->insert(rand());

	//Reinitialize rand();
	srand( (unsigned)time( NULL ) );

	//Search for 20 random keys using serial search
	for(i=0; i<20; i++)
	{
		j=rand();
		cout<<"Searching tree for key "<<j<<"..."<<endl;
		check=tree->BinarySearchKey(j, &index, &accesses);
		if(check==NULL)
			cout<<"Not Found!"<<endl<<endl;
		else
			cout<<"Found! (Node: "<<check<<",  Index: "<<index<<")"<<endl;
	}

	//Reinitialize rand();
	srand( (unsigned)time( NULL ) );

	index=-1;

	//Search for 20 random keys using binary search
	for(i=0; i<20; i++)
	{
		j=rand();
		cout<<"Searching tree for key "<<j<<"..."<<endl;
		check=tree->BinarySearchKey(j, &index, &binAccesses);
		if(check==NULL)
			cout<<"Not Found!"<<endl<<endl;
		else
			cout<<"Found! (Node: "<<check<<",  Index: "<<index<<")"<<endl;
	}

	//Print stats
	cout<<endl<<"Total memory accesses using serial search: "<<accesses;
	cout<<endl<<"Total memory accesses using binary search: "<<binAccesses<<endl<<endl;

	//Program ends here.
	return;
}