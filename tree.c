/* tree.c                   
 * Roderick "Rance" White
 * roderiw
 * Lab5: Binary Search Trees
 * ECE 2230, Fall 2020
 *
 * Propose: Functions for the binary trees.
 *
 * Assumptions: 
 *
 * Bugs: AVL has memory leakage for equillibrium
 *
 */

#include <stdio.h>
#include <string.h>


#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "tree.h"


//Private function prototypes
void ugly_print(TreeNode *N, int level);
int tree_debug_validate_rec (TreeNode *N, int min, int max, int *count);

void recursiveTreeDelete(TreeNode *N);

TreeNode *BSTSearch (Tree *T, TreeNode *N, TreeNode *Holder, int Operation, int *KR);
TreeNode *AVLSearch (Tree *T, TreeNode *N, TreeNode *Holder, int Operation, int *KR);
TreeNode *TreeNodeRemove (Tree *T, TreeNode *N);

TreeNode *RotateLeft(TreeNode *TopNode);
TreeNode *RotateRight(TreeNode *TopNode);

int TreeIPCount(TreeNode *N, int count);



/* Create the header block for the tree and save the BalanceOption in
 * the header block. The BalanceOption must be one of the labels
 * defined with an enum as shown above. While the definition allows
 * for multiple types of trees, you are only required to implement the
 * BST option. 
 *
 * Initialize the root pointer to NULL. 
 * 
 * The ItemCount stores the current number of keys in the tree.
 * 
 * The RecentComparisonCount stores the number of key comparisons
 * during the most recent search, insert, or remove.
 * 
 * Use Standish's definitions for the number of comparisons even if
 * your implementation is slightly different. That is, there is one
 * comparison to determine if the key is found at the current level
 * and if the key is not found one more comparison to determine if
 * the next step is to the left or the right. Do not count checks 
 * for NULL pointers
 */
Tree *tree_construct (int BalanceOption)
{
	/* Assert that the balance option is one of the balanceoptions */
	assert(BalanceOption == AVL || BalanceOption == BST || BalanceOption == TWOTHREET);

	Tree *T;
	T = (Tree *) malloc(sizeof(Tree));	//Get size for new tree
	T->Root = NULL;
	T->BalanceOption = BalanceOption;
	T->ItemCount = 0;
	T->RecentComparisonCount = 0;

	return T;
}

/* Frees all items stored in the tree including the memory block
 * with the data and the TreeNode structure. 
 *
 * Also frees the header.
 */
void tree_destruct (Tree *T)
{
	if(T != NULL && T->Root != NULL)			//Only do this is T isn't already empty
		recursiveTreeDelete(T->Root);			//Free the nodes in T before freeing T
	free(T);									//Finally, free T
}

/* Recursive removal of all nodes before freeing of tree */
void recursiveTreeDelete(TreeNode *N)
{

	/* Remove data in leaves before freeing the node */
	if(N->LeftLink != NULL)
	{
		recursiveTreeDelete(N->LeftLink);		//Remove data in left leaf
		N->LeftLink = NULL;
	}
	if(N->RightLink != NULL)
	{
		recursiveTreeDelete(N->RightLink);		//Remove data in right leaf
		N->RightLink = NULL;
	}
	free(N->DataPtr);
	free(N);									//Remove data in node N

}


/* Insert the memory block pointed to by DataPointer into the tree
 * with the associated key.
 * 
 * The function must return 0 if the key is already in the key (in
 * which case the data memory block is replaced).
 * 
 * The function must return 1 if the key was not already in the tree
 * but was instead added to the tree.
 * 
 * You are required to implement the BST policy only. If you elect 
 * to implement more than one type of tree, then the insertion 
 * method should conform to the BalanceOption that is defined when
 * the tree was initially constructed.
 */
int tree_insert (Tree *T, KeyType Key, DataType DataPointer)
{
	int KeyReturn;

	T->RecentComparisonCount=1;						//Reset to 1

	/* Create new TreeNode for the key to be inserted */
	TreeNode *NewNode;
	NewNode = (TreeNode *) malloc(sizeof(TreeNode));
	NewNode->NodeItem = Key;
	NewNode->DataPtr = DataPointer;
	NewNode->height=1;					//Only useful for AVL Trees
	NewNode->LeftLink = NULL;
	NewNode->RightLink = NULL;


	/* Insert the new node based on the BalanceOption given to the tree T */
	/* BST Insert */
	if(T->BalanceOption == BST)
		T->Root = BSTSearch (T, T->Root, NewNode, Insert, &KeyReturn);

	/* AVL Insert */
	if(T->BalanceOption == AVL)
	{
		T->Root=AVLSearch(T, T->Root, NewNode, Insert, &KeyReturn);
	}

	return KeyReturn;
}




/* Removes the item in the tree with the matching key.
 * 
 * Return the pointer to the data memory block and free the TreeNode
 * memory block. If the key is not found in the tree, return NULL.
 * 
 * You are required to implement the BST policy only. If you elect 
 * to implement more than one type of tree, then the insertion 
 * method should conform to the BalanceOption that is defined when
 * the tree was initially constructed.
 */
DataType tree_remove (Tree *T, KeyType Key)
{
	int KeyReturn;
	DataType RemovedDataType;

	/* Create new TreeNode so DataPtr can be returned */
	TreeNode *NewNode;
	NewNode = (TreeNode *) malloc(sizeof(TreeNode));
	NewNode->NodeItem = Key;
	NewNode->DataPtr = 0;
	NewNode->height=0;
	NewNode->LeftLink = NULL;
	NewNode->RightLink = NULL;

	/* Insert the new node based on the BalanceOption given to the tree T */
	/* BST Remove */
	if(T->BalanceOption == BST)
		T->Root = BSTSearch (T, T->Root, NewNode, Remove, &KeyReturn);

	/* AVL Remove */
	if(T->BalanceOption == AVL)
	{
		T->Root=AVLSearch(T, T->Root, NewNode, Remove, &KeyReturn);
	}

	RemovedDataType = NewNode->DataPtr;
	free(NewNode);

	return RemovedDataType;
}




/* Find the tree element with the matching key and return the 
 * pointer to the data block that is stored in this node in the tree.
 * If the key is not found in the tree then return NULL.
 */
DataType tree_search (Tree *T, KeyType Key)
{
	int KeyReturn;
	DataType FoundDataType;

	/* Create new TreeNode so DataPtr can be returned */
	TreeNode *NewNode;
	NewNode = (TreeNode *) malloc(sizeof(TreeNode));
	NewNode->NodeItem = Key;
	NewNode->DataPtr = 0;
	NewNode->height=0;
	NewNode->LeftLink = NULL;
	NewNode->RightLink = NULL;

	T->Root = BSTSearch (T, T->Root, NewNode, Search, &KeyReturn);

	FoundDataType = NewNode->DataPtr;
	free(NewNode);

	return FoundDataType;
}

/* Return the number of keys in the tree. */
int tree_count (Tree *T)
{
	return T->ItemCount;
}

/* Return RecentComparisonCount, the number of key comparisons for 
 * the most recent call to tree_insert, tree_remove, or tree_search.
 */
int tree_stats (Tree *T)
{
	return T->RecentComparisonCount;
}

/* Return the internal path length to the tree */
int tree_internal_path_len (Tree *T)
{
	return TreeIPCount(T->Root, 0);
}

int TreeIPCount(TreeNode *N, int count)
{
	int PrevCount = count+1;
	if(N == NULL)
		return 0;
	if(N->LeftLink != NULL)
		count = count + TreeIPCount(N->LeftLink, PrevCount);
	if(N->RightLink != NULL)
		count = count + TreeIPCount(N->RightLink, PrevCount);
	return count;
}



/* Debug Functions */


void ugly_print(TreeNode *N, int level)
{
	if (N == NULL) return;
	ugly_print(N->RightLink, level+1);
	for (int i=0; i<level; i++) printf("     ");	//5 spaces
	printf("%5d,%d\n", N->NodeItem,N->height);					//field width is 5
//	printf("%5d\n", N->NodeItem);					//field width is 5
	ugly_print(N->LeftLink, level+1);
}




void tree_debug_print_tree (Tree *T)
{
	printf("Tree with %d keys\n", T->ItemCount);
	ugly_print(T->Root, 0);
	printf("\n");
	tree_debug_validate(T);
}




int tree_debug_validate_rec (TreeNode *N, int min, int max, int *count)
{
	if (N == NULL) return TRUE;
	if (N->NodeItem <= min || N->NodeItem >= max) return FALSE;
	assert(N->DataPtr != NULL);
	*count += 1;
	return tree_debug_validate_rec(N->LeftLink, min, N->NodeItem, count) && 
		tree_debug_validate_rec(N->RightLink, N->NodeItem, max, count);
}


void tree_debug_validate (Tree *T)
{
	int size = 0;
	assert(tree_debug_validate_rec(T->Root, INT_MIN, INT_MAX, &size) == TRUE);
	assert(size == T->ItemCount);

}















/*---------------------------------------------------------------------------------------------

Binary Search Tree Functions

---------------------------------------------------------------------------------------------*/

/* Recursive function for BST operations */
TreeNode *BSTSearch (Tree *T, TreeNode *N, TreeNode *Holder, int Operation, int *KR)
{

	/* If lowest level of branch is reached */
	if(N == NULL)
	{
		*KR = 1;
		if(Operation == Insert)				//Insert Holder at lowest level if insert operation
		{
			T->ItemCount=T->ItemCount+1;	//There is one more item in the tree
			return Holder;
		}
		else								//Make Holder NULL to return if removing or searching
			Holder=NULL; 
		return NULL;
	}

	T->RecentComparisonCount=T->RecentComparisonCount+2;	//Increase comparison by two
	/* If the new key is less than the current key, check the left leaf */
	if(Holder->NodeItem < N->NodeItem)
		N->LeftLink = BSTSearch(T, N->LeftLink, Holder, Operation, KR);

	/* If the new key is greater than the current key, check the right leaf */
	else if(Holder->NodeItem > N->NodeItem)
		N->RightLink = BSTSearch(T, N->RightLink, Holder, Operation, KR);

	/* If the Key is equal, */
	else
	{
		*KR = 0;
		if(Operation == Insert)			//If inserting, replace the old node with the Holder node
		{
			/* Place necessary information from old node into new node */
			Holder->height=N->height;
			Holder->LeftLink = N->LeftLink;
			Holder->RightLink = N->RightLink;
			free(N->DataPtr);			//Must free datapointer before freeing node
			free(N);
			return Holder;
		}
		else if(Operation == Search)	//If searching, place DataPtr of matching node in Holder
		{
			Holder->DataPtr = N->DataPtr;
			return N;
		}
		else if(Operation == Remove)	//If removing, remove matching node and return its DataPtr
		{
			Holder->DataPtr = N->DataPtr;
			T->ItemCount=T->ItemCount-1;			//There is one less item in the tree
			return TreeNodeRemove (T, N);
		}
		else return NULL;
	}
	return N;
}

/* Removal of node */
TreeNode *TreeNodeRemove (Tree *T, TreeNode *N)
{
	TreeNode *PrevNode, *RepNode;

	/* If only one leaf exists */
	//If there's only a right leaf, make the right leaf replace the removed node
	if(N->LeftLink == NULL)
	{
		/* Change next pointer */
		RepNode = N->RightLink;			//Give next leaf to RepNode before free(N) to be returned
		free(N);
		return RepNode;					//Must be returned to replace N
	}
	//If there's only a left leaf, make the left leaf replace the removed node
	else if(N->RightLink == NULL)
	{
//Error where the height doesn't change
		/* Change next pointer */
		RepNode = N->LeftLink;			//Give next leaf to RepNode before free(N) to be returned
		free(N);
		return RepNode;					//Must be returned to replace N
	}
	/* If there are two leaves, find the leaf that will replace the removed node N */
	else
	{
		PrevNode = N;
		RepNode = N->RightLink;

		//Find node to replace the removed node
		while(RepNode && RepNode->LeftLink != NULL)
		{
			PrevNode = RepNode;
			RepNode = RepNode->LeftLink;
		}

		/* Replace values in removed node */
		N->NodeItem = RepNode->NodeItem;
		N->DataPtr = RepNode->DataPtr;

		// If the previous node is the removed node
		if(PrevNode->RightLink == RepNode) PrevNode->RightLink = RepNode->RightLink;
		else PrevNode->LeftLink = RepNode->RightLink;

		free(RepNode);
	}
	return N;
}










/*---------------------------------------------------------------------------------------------

AVL Functions

---------------------------------------------------------------------------------------------*/


TreeNode *RotateLeft(TreeNode *TopNode)
{
printf("\n\nRotate left\n\n");
	int LHeight = 0, RHeight = 0;

	TreeNode *RNode = TopNode->RightLink;			//Node to the right to be put on top
	TreeNode *TransLeaf = RNode->LeftLink;			//Leaf between the two nodes to be moved.

	RNode->LeftLink = TopNode;
	TopNode->RightLink = TransLeaf;

	/* Find largest height for the old top node */
	if(TopNode->LeftLink != NULL) LHeight = TopNode->LeftLink->height;
	if(TopNode->RightLink != NULL) RHeight = TopNode->RightLink->height;
	if(RHeight > LHeight) TopNode->height = RHeight + 1;
	else TopNode->height = LHeight + 1;

	/* Find largest height for the old right node */
	//Reset to 0
	LHeight = 0;
	RHeight = 0;
	if(RNode->LeftLink != NULL) LHeight = RNode->LeftLink->height;
	if(RNode->RightLink != NULL) RHeight = RNode->RightLink->height;
	if(RHeight > LHeight) RNode->height = RHeight + 1;
	else RNode->height = LHeight + 1;

	return RNode;
}


TreeNode *RotateRight(TreeNode *TopNode)
{
printf("\n\nRotate right\n\n");
	int LHeight = 0, RHeight = 0;

	TreeNode *LNode = TopNode->LeftLink;			//Node to the left to be put on top
	TreeNode *TransLeaf = LNode->RightLink;			//Leaf between the two nodes to be moved.

	LNode->RightLink = TopNode;
	TopNode->LeftLink = TransLeaf;

	/* Find largest height for the old top node */
	if(TopNode->LeftLink != NULL) LHeight = TopNode->LeftLink->height;
	if(TopNode->RightLink != NULL) RHeight = TopNode->RightLink->height;
	if(RHeight > LHeight) TopNode->height = RHeight + 1;
	else TopNode->height = LHeight + 1;

	/* Find largest height for the old left node */
	//Reset to 0
	LHeight = 0;
	RHeight = 0;
	if(LNode->LeftLink != NULL) LHeight = LNode->LeftLink->height;
	if(LNode->RightLink != NULL) RHeight = LNode->RightLink->height;
	if(RHeight > LHeight) LNode->height = RHeight + 1;
	else LNode->height = LHeight + 1;

	return LNode;
}




/* Recursive function for AVL operations */
TreeNode *AVLSearch (Tree *T, TreeNode *N, TreeNode *Holder, int Operation, int *KR)
{
	int LHeight = 0, RHeight = 0, BF = 0;
	/* If lowest level of branch is reached */
	if(N == NULL)
	{
		*KR = 1;
		if(Operation == Insert)
		{
			T->ItemCount=T->ItemCount+1;
			return Holder;
		}
		else
			Holder=NULL; 
		return NULL;
	}

	T->RecentComparisonCount=T->RecentComparisonCount+2;

	if(Holder->NodeItem < N->NodeItem)
		N->LeftLink = AVLSearch(T, N->LeftLink, Holder, Operation, KR);

	else if(Holder->NodeItem > N->NodeItem)
		N->RightLink = AVLSearch(T, N->RightLink, Holder, Operation, KR);

	/* If the Key is equal, */
	else
	{
		*KR = 0;
		if(Operation == Insert)
		{
			/* Place necessary information from old node into new node */
			Holder->height=N->height;
			Holder->LeftLink = N->LeftLink;
			Holder->RightLink = N->RightLink;
			free(N->DataPtr);
			free(N);
			return Holder;
		}
		else if(Operation == Search)
		{
			Holder->DataPtr = N->DataPtr;
			return N;
		}
		else if(Operation == Remove)
		{
			Holder->DataPtr = N->DataPtr;
			T->ItemCount=T->ItemCount-1;
			return TreeNodeRemove (T, N);
		}
	}
//printf("Wow");

	/* Only update variable to hold height if it isn't NULL */
	if(N->LeftLink != NULL)
		LHeight = N->LeftLink->height;
	if(N->RightLink != NULL)
		RHeight = N->RightLink->height;

	/* The new size of the node is the height of its largest descendant plus 1 */
	if(RHeight > LHeight) N->height = RHeight + 1;
	else N->height = LHeight + 1;

	BF = RHeight - LHeight;
//printf("BF: %d, NodeItem: %d, Holder: %d\n", BF, N->NodeItem, Holder->NodeItem);
//tree_debug_print_tree(T);
	/* Rotate right if Left Height is more than 1 larger than the Right Height */
	if(BF < -1)
	{
		//Double right rotate
		if(Holder->NodeItem > N->LeftLink->NodeItem) N->LeftLink = RotateLeft(N->LeftLink);

		return RotateRight(N);
	}
	/* Rotate left if Right Height is more than 1 larger than the Left Height */
	if(BF > 1)
	{
		//Double left rotate
		if(Holder->NodeItem < N->RightLink->NodeItem){
//			printf("Rightnode: %d\n",N->RightLink->NodeItem); 
			N->RightLink = RotateRight(N->RightLink);}

		return RotateLeft(N);
	}

	return N;
}

























