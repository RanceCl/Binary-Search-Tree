/* tree.h
 * Roderick "Rance" White
 * roderiw
 * Lab5: Binary Search Trees
 * ECE 2230, Fall 2020
 */

/* constants used with Global variables */

#define TRUE 1
#define FALSE 0

enum balanceoptions {AVL, BST, TWOTHREET};
enum treeoperations {Insert, Remove, Search};

//enum BalanceFactor {LeftHeavy, Balanced, RightHeavy};

typedef int KeyType;
typedef void *DataType;

typedef struct NodeTag {
	KeyType NodeItem;
	DataType DataPtr;
        int height;
//        BalanceFactor BF;
	struct NodeTag *LeftLink;
	struct NodeTag *RightLink;
} TreeNode;
/*
typedef struct AVLNodeTag {
	KeyType NodeItem;
	DataType DataPtr;
        BalanceFactor BF;
	struct NodeTag *LeftLink;
	struct NodeTag *RightLink;
} AVLTreeNode;
*/
typedef struct TreeTag {
	TreeNode *Root;
	int BalanceOption;		// must be a balanceoptions
	int ItemCount;
	int RecentComparisonCount;
} Tree;


/* Creates the tree header block */
Tree *tree_construct (int BalanceOption);

/* Frees all items stored in the tree and the header */
void tree_destruct (Tree *);

/* Insert the memory block pointed to by DataPointer into the tree */
int tree_insert (Tree *, KeyType Key, DataType DataPointer);

/* Removes the item in the tree with the matching key */
DataType tree_remove (Tree *, KeyType Key);

/* Find the tree element with the matching key
 * return the pointer to the data block 
 */
DataType tree_search (Tree *, KeyType Key);

/* Return the number of keys in the tree */
int tree_count (Tree *);

/* Return RecentComparisonCount */
int tree_stats (Tree *);

/* Return the internal path length to the tree */
int tree_internal_path_len (Tree *);

/* Debug Functions */
void tree_debug_print_tree (Tree *);
void tree_debug_validate (Tree *);


/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
