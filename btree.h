#include <iostream>
using namespace std;

class BTreeNode
{
	int *keys;	// array of keys
	int t;		// max degree
	BTreeNode **C; // array of child pointers
	int n;		// current number of keys
	bool leaf;	// is true when node is leaf
public:
	BTreeNode(int _t, bool _leaf);	// ctr
	void traverse();	// fn to traverse all nodes in a subtree
	BTreeNode *search(int k ); // fn to search a key in subtree, returns NULL if k is not present
	BTreeNode *insert(int num);

friend class Btree;
};

class BTree
{
	BTreeNode *root;
	int t;		// max degree
public:
	BTree(int _t) { root = NULL; t = _t; }
	void traverse() { if (root) root->traverse(); }
	BTreeNode* search(int k) { return (root)? root->search(k) : NULL; }
	bool insert(int num); // { return (root)? root->insert(num) : false; }
};

