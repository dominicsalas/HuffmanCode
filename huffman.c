#include <stdio.h>
#include <math.h>
#include "huffman.h"

struct TreeNode
{
	unsigned long count;
	unsigned char symbol;
	//Not used
	//struct TreeNode* left;
	struct TreeNode* right;
};

struct sumRoot
{
	unsigned long sum;
	struct sumRoot* left;
	struct sumRoot* right;
	struct sumRoot* next;
};

typedef struct TreeNode TreeNode;
typedef struct sumRoot sumRoot;

TreeNode* createNode(unsigned long count, unsigned char symbol)
{
	TreeNode* tree = malloc(sizeof (TreeNode));
	tree->count = count;
	tree->symbol = symbol;
	//Not used
	//tree->left = NULL;
	tree->right = NULL;
	return tree;
}

sumRoot* createsumRoot(unsigned long sum)
{
	sumRoot* tree = malloc(sizeof(sumRoot));
	tree->sum = sum;
	tree->left = NULL;
	tree->right = NULL;
	tree->next = NULL;
}

// This is a function to construct leaf nodes from symbols 
// and weights (count)
/*
TreeNode* buildNodes(unsigned long count[], unsigned char ascii[])
{
	int i;
	TreeNode* root = NULL;

	for (i = 0; i < 256; i++)
	{
		if (count[i] > 0)
		{
			TreeNode* root = NULL;
			root = createNode(count[i], ascii[i]);
		}
			//TreeNode* root = NULL;
	}

	return root;
}
*/

// Function to build the linked list of leaf nodes
TreeNode* insertSorted(TreeNode* head, unsigned long count, unsigned char symbol)
{
	TreeNode* current = head;
	TreeNode* newNode = createNode(count, symbol);

	if (current == NULL || count < current->count)
	{
		newNode->right = current;
		return newNode;
	}
	else
	{
		while (current->right != NULL && current->right->count <= count)
		{
			if (current->right->count < count)
			{
				current = current->right;
			}
			else
			{
				if (current->right->symbol < symbol)
				{
					current = current->right;
				}
				else
				{
					break;
				}
			}
		}
		newNode->right = current->right;
		current->right = newNode;
		return head;
	}
}

// This is a function to convert linked list to binary tree
TreeNode* listtoTree(TreeNode* head)
{
	TreeNode* current = head;
	TreeNode* newHead = head;
	TreeNode* newNode = createNode(NULL, NULL);
	unsigned long sum = 0;

	while (current->right != NULL)
	{
		sum = current->count + current->right->count;
		newNode->count = sum;

		newNode->left = current;
		newNode->right = current->right;
		newHead = current->right->right;
		current->right = NULL;
		current->right->right = NULL;
		listtoTree(newHead);
	}

	return head;
}

// This is a function to print out the nodes with their 
// symbols and weigts
/*
void printLeavesHelper(TreeNode* root)
{
	if (root != NULL)
	{
		printLeavesHelper(root->left);
		printLeavesHelper(root->right);
		if (root->left == NULL && root->right == NULL)
		{
			printf("%c:", root->symbol);
			printf("%d ", root->count);
		}
	}
}
*/

void printList(TreeNode* head)
{
	TreeNode* current = head;
	while (current != NULL)
	{
		printf("%c:", current->symbol);
		printf("%d ", current->count);
		current = current->right;
	}
	printf("\n");
}

/*
void printLeaves(TreeNode* root)
{
	printLeavesHelper(root);
	printf("\n");
}
*/

/* Free function but not quite working!
void freeTree(TreeNode* root)
{
	if (root != NULL)
	{
		freeTree(root->left);
		freeTree(root->right);
		freeTree(root);
	}
}
*/

// Enumerate all 256 ascii characters
void enumerateascii(unsigned char ascii[])
{
	int i;

	for (i = 0; i < 256; i++)
	{
		ascii[i] = i;
	}
}

// Zero out the counter array
void zerocounter(unsigned long count[])
{
	int i;

	for (i = 0; i < 256; i++)
	{
		count[i] = '\0';
	}
}

void encodeFile(FILE* in, FILE* out, int printTable)
{
	char c;
	int i = 0;
	int j = 0;	// Not using so far
	int num_bits = 0;		// Count number of bits to use for frequency code
	int counter = 0;	// Will be used to count unique chars
	double bin_count = 0; // Binary counter
	unsigned long count[256];
	unsigned char ascii[256];

	enumerateascii(ascii);	// Build 256 ascii array
	zerocounter(count);		// Set 256 counter array to NULL
	
	// Initial character grab
	c = fgetc(in);
	// While loop to count characters
	while (c != EOF)
	{
		for (i = 0; i < 256; i++)
		{
			if (c == ascii[i])
			{
				count[i]++;	// Increments number of characters at each one found
				if (count[i] == 1)
				{
					counter++;
				}
				break;
			}
		}
		c = fgetc(in);
	}

	// Calculates number of bits needed
	for (i = 0; i <= 8; i++)
	{
		bin_count = pow(2, i);
		if (bin_count > counter)
		{
			break;
		}
		num_bits++;
	}

	TreeNode* head = NULL;

	// Build leaf nodes and sort them
	for (i = 0; i < 256; i++)
	{
		if (count[i] > 0)
		{
			head = insertSorted(head, count[i], ascii[i]);
		}
	}

	// For debugging. Prints out sorted linked list
	printf("Symbols: ");
	printList(head);

	listtoTree(head);

	//TreeNode* tree = buildNodes(count, ascii);
	//printLeaves(tree);

	// Free Function, but not quite working
	//freeTree(tree);
}