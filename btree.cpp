#include "btree.h"

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
		temp = root->insert(num, root);
	} else {	// root is NULL, allocate new BTreeNode
		root = new BTreeNode(t, true);
		root->insert(num, root);
	}

	// If temp is non-empty, that means BTreeNode::insert() split the root node. A new root node
	// was created, therefore the BTree root pointer must be updated to point to that node.
	if(temp) {
		root = temp;
	}

	return rc;
}
