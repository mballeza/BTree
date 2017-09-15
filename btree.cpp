#include "btree.h"

BTreeNode::BTreeNode(int _t, bool _leaf)
{
	// copy given min degree and leaf property
	t = _t;
	leaf = _leaf;

	// allocate memory for max number of possible keys and child pointers
	keys = new int[t-1];
	C = new BTreeNode *[t];

	// init number of keys as 0
	n = 0;
}

void BTreeNode::traverse()
{
	// There are n keys and n+1 children, traverse through n keys and first n children
	int i;
	for (i = 0; i < n; i++) {
		// if not a leaf, then before printing key[i],
		// traverse the subtree rooted with child C[i]
		if (!leaf)
			C[i]->traverse();
		cout << " " << keys[i];
	}

	// print the subtree rooted with last child
	if (!leaf)
		C[i]->traverse();
}

BTreeNode *BTreeNode::search(int k)
{
	// find the first key greater than or equal to k
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	// if the found key is equal to k, return this node
	if (keys[i] == k)

	// if key is not found here and this is a leaf node
	if (leaf)
		return NULL;

	// go to appropriate child
	return C[i]->search(k);
}

// Insertion function. Insertion follows these rules:
//  1. If there is room in a node, where n < t-1, insert simply.
//  2. If n == t-1 after insertion, the key at t/2 is sent up to the parent node. The two partitions
//    are then split into two child nodes.
//    a. All keys less than the promoted key go to one node, where all keys at < t/2 become children 
//      of the parent node at the same array position as the promoted key. 
//      i. For example, a promoted key is inserted at the m-th element of the parent, then
//        the child pointer for the nodes at < t/2 of the child node is at m, i.e. C[m].
//      ii. All child pointers at less than t/2 belong to the same node as these keys.
//        - Note: leaves do not have child pointers.
//    b. All keys greater than the promoted key go to another node, where all keys at >= t/2 become
//      children of the parent node at the position C[m+1].
//      i. All child pointers at greater than t/2 belong to the same node as these keys.
//        - Note: leaves do not have child pointers.
//    c. This effect may ripple up to the root node.
//      i. If the root node is split, update the promoted key as root.
//
// Edge cases:
//  1. Empty node, n == 0.
//    a. Insert at keys[0].
//  2. Non-empty node, n > 0 && n < t-1.
//    a. Iterate through keys. Insert where num < current_key. Increment current_key array 
//      element and each subsequent key array elements.
//  3. Before insertion.
//    a. n < t-1
//      i. No modification to child pointers.
//    b. n == t-1 
//      i. This violates the invariance of the btree. Insertion should never happen on a node
//        with t-1 keys already in place.
//      ii. TODO: implement panic message for this case.
//  4. After insertion.
//    a. n < t-1
//      i. No modification to child pointers.
//    a. n == t-1
//      i. The max number of keys per node has been reached. The node must be split.
//
// Parameters :  Integer value to insert into the btree.
// Returns    :  New root node.
//               - If the insertion ripple effect reaches the root node, return the new node created
//                 from the split.
// Otherwise  :  NULL
BTreeNode *BTreeNode::insert(int num)
{
	BTreeNode *temp = NULL;

// Before insertion conditons:
	if(n == 0) {

	} else if(n > 0 && n < t-1) {

	} else if(n >= t-1) {
		// throw message, see Edge case 3b
		throw ("***Number of keys already reached limit at ", __FILE__, __LINE__);
	}

// After insertion
	if(n == t-1) {
		// split node
	}

	return temp;
}

// Insertion function. See BTreeNode::insert() for more details.
//
// - Edge cases:
//   - Empty root.
//     - Allocate space for root. Call BTreeNode::insert().
//   - Non-empty root.
//     - Call BTreeNode::insert().
// Returns    :  True 
//               - Insertion is successful. 
//               - The integer num is out of some range (may implement later). 
// Otherwise  :  False
bool BTree::insert(int num)
{
	BTreeNode *temp = NULL;
	bool rc = true;

	if(root) {	// root exists, call BTreeNode::insert()
		temp = root->insert(num);
	} else {	// root is NULL, allocate new BTreeNode
		root = new BTreeNode(t, true);
		root->insert(num);
	}

	// If temp is non-empty, that means BTreeNode::insert() split the root node. A new root node
	// was created, therefore the BTree root pointer must be updated to point to that node.
	if(temp) {
		root = temp;
	}

	return rc;
}
