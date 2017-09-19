#include "btree.h"

BTreeNode::BTreeNode(int _t, bool _leaf)
{
	// copy given min degree and leaf property
	t = _t;
	leaf = _leaf;

	// allocate memory for max number of possible keys and child pointers
	keys = new int[t-1];
	for(int i = 0; i < t-1; i++)
		keys[i] = 0;

	C = new BTreeNode *[t];
	for(int i = 0; i < t; i++)
		C[i] = NULL;

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
//               Parent node by reference.
// Returns    :  New root node.
//               - If the insertion ripple effect reaches the root node, return the new node created
//                 from the split.
// Otherwise  :  NULL
BTreeNode *BTreeNode::insert(int num, BTreeNode *&parent)
{
	BTreeNode *thisNode = this;
	BTreeNode *temp = NULL;

	// check if leaf
	//  - if yes, insert num, increment n
	//  - if no, call insert function on the proper child pointer
	if(leaf) {
// Before insertion conditons:
		if(n == 0) {
			// insert num
			keys[0] = num;

			// increment n
			n++;
		} else if(n > 0 && n < t-1) {
			insertKey(num);
			/* moved to insertKey()
			int i = 0; // current index
			// insert num
			for(i = 0; i < t-1; i++) {
				if(num <= keys[i]) {
					int swp;

					// copy subsequent keys and increment their index by 1
					//
					// for loop
					//  - start at index n, this should not contain a key
					//  - copy previous index into current
					//  - decrement index
					//  - repeat until j == i
					for(int j = n; j > i; j--) {
						keys[j] = keys[j-1];
					}

					// insert num into keys[i]
					keys[i] = num;
				}
			}
			*/

			// increment n
			n++;
		} else if(n >= t-1) {
		// throw message, see Edge case 3b
		throw ("***Number of keys already reached limit at ", __FILE__, __LINE__);
		}
// End before insertion conditions
	} else {
		int i = 0; // current index

		while(C[i] && i < t-1) {
			// if num > keys[i], go to next index
			if(num <= keys[i]) {
				C[i]->insert(num, thisNode);
			}
			i++;
		}

		if(i == t-1) {
			C[i]->insert(num, thisNode);
		}
	}

// After insertion
	if(n == t-1) {
		// split node
		//   - This node keeps all keys at < t/2, n becomes t/2 - 1
		//   - Allocate new node (temp) to put keys at > t/2, with n = t - t/2 - 1
		//   - Update parent node
		//     - Insert key
		//     - Add child pointer
		//     - Increment n
		//   - Update leaf value

		if(parent) {	// basically if not root
			BTreeNode *temp_child = new BTreeNode(t, false);
			BTreeNode *empty = NULL;	// hack, fixes non-const reference error

			// if leaf, no inheritance of child pointers, otherwise temp_child inherits
			//
			// temp_child inherits nodes at >= t/2
			//  - thisNode's child pointers are erased at >= t/2
			// thisNode keeps child pointers at < t/2
			if(leaf) {
			} else {
			}

		} else {		// if root
			BTreeNode *temp_parent = new BTreeNode(t, false);
			BTreeNode *temp_child = new BTreeNode(t, true);
			BTreeNode *empty = NULL;	// hack, fixes non-const reference error

			// insert key at t/2 into new parent (root) node
			//temp_parent->insert(keys[t/2], empty);

			// add child pointer, TODO: make function
			temp_parent->updateRootParent(keys[t/2], thisNode, temp_child);
			
			// move keys at greater than t/2 to new child node
			for(int i = t/2 + 1; i < t; i++) {
				temp_child->insert(keys[i], temp_parent);
			}

			// TODO: erase comment
			// thisNode->eraseKeys(0, t/2);

			temp = temp_parent;	// assign new parent node to return as new root }
		}
	}
// End after insertion

	return temp;	// returns NULL if root is not split, otherwise returns address of new root
}

void BTreeNode::eraseKeys(int start, int end)
{
	for(int i = start; i < end; i++) {
		keys[i] = 0;
		n--;
	}
}

void BTreeNode::eraseNodes(int start, int end)
{
	for(int i = start; i < end; i++) {
		delete C[i];
		C[i] = NULL;
	}
}

void BTreeNode::updateNonRootParent(int num, BTreeNode *&child)
{
	// insert num into appropriate place in keys array
	insertKey(num);

	// add child pointer to the next available index
	//  - add a throw case for currently full C array
	int i = 0; // current index

	while(C[i] && i < t-1) {
		i++;
	}

	if(C[i]) {
		throw ("ERROR: child pointer is already non empty at ", __FILE__, __LINE__);
	} else {
		C[i] = new BTreeNode(t, false);
		C[i] = child;	// TODO: needs hard copy assignment operator overload fn
	}
}

void BTreeNode::updateRootParent(int num, BTreeNode *&left, BTreeNode *&right)
{
	// insert num into keys[0]
	keys[0] = num;

	// add left pointer to C[0] and right pointer to C[1]
	C[0] = left;
	C[1] = right;
}

void BTreeNode::insertKey(int num)
{
	int i = 0; // current index
	for(i = 0; i < t-1; i++) {
		if(num <= keys[i]) {
			int swp;

			// copy subsequent keys and increment their index by 1
			//
			// for loop
			//  - start at index n, this should not contain a key
			//  - copy previous index into current
			//  - decrement index
			//  - repeat until j == i
			for(int j = n; j > i; j--) {
				keys[j] = keys[j-1];
			}

			// insert num into keys[i]
			keys[i] = num;
		}
	}
}
