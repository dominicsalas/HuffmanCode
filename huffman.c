#include <stdio.h>
#include <math.h>
#include "huffman.h"

//This is a struct for the leaf nodes
struct TreeNode
{
	unsigned char symbol;
	unsigned long weight;
	struct TreeNode* next;
	struct TreeNode* left;
	struct TreeNode* right;
};

// This is a struct for the Sum Roots that will be used
// in creating the binary tree
struct sumRoot
{
	unsigned long sum;
	struct sumRoot* left;
	struct sumRoot* right;
	struct sumRoot* next;
};

struct LeafNode
{
	unsigned char symbol;
	unsigned long weight;
};

typedef struct TreeNode TreeNode;
typedef struct sumRoot sumRoot;
typedef struct LeafNode LeafNode;

TreeNode* createNode(unsigned char symbol, unsigned long weight)
{
	TreeNode* tree = malloc(sizeof (TreeNode));
	tree->symbol = symbol;
	tree->weight = weight;
	tree->next = NULL;
	tree->left = NULL;
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

LeafNode* createLeafNode(unsigned char symbol, unsigned long weight)
{
	LeafNode* node = malloc(sizeof (LeafNode));
	node->symbol = symbol;
	node->weight = weight;
}

// Function to build the linked list of leaf nodes
TreeNode* insertSorted(TreeNode* head, unsigned long count, unsigned char symbol)
{
	TreeNode* current = head;
	TreeNode* newNode = createNode(symbol, count);

	if (current == NULL || count < current->weight)
	{
		newNode->next = current;
		return newNode;
	}
	else
	{
		while (current->next != NULL && current->next->weight <= count)
		{
			if (current->next->weight < count)
			{
				current = current->next;
			}
			else
			{
				if (current->next->symbol < symbol)
				{
					current = current->next;
				}
				else
				{
					break;
				}
			}
		}
		newNode->next = current->next;
		current->next = newNode;
		return head;
	}
}

// This is a function to convert linked list to binary tree
TreeNode* listtoTree(TreeNode* head)
{
	unsigned long sum = 0;
	TreeNode* current = head;
	//sumRoot* root = head;
	TreeNode* left = createNode(current->symbol, current->weight);
	TreeNode* right = createNode(current->next->symbol, current->next->weight);
	//sumRoot* right = createsumRoot(current->right->count);
	
	//sum = current->count + current->right->count;
	sum = left->weight + right->weight;
	//sumRoot* root = createsumRoot(sum);
	TreeNode* root = createNode(NULL, sum);
	//left = current;
	//right = current->right;

	root->left = left;
	root->right = right;
	root->next = current->next->next;

	//current = 

	current = current->next->next;

	sum = current->weight + current->next->weight;
	root = createsumRoot(sum);

	root->left = current;
	root->right = current->next;

	/*
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
	*/

	return head;
}

// This is a function to print out the nodes with their 
// symbols and weigts
void printList(TreeNode* head)
{
	TreeNode* current = head;
	while (current != NULL)
	{
		printf("%c:", current->symbol);
		printf("%d ", current->weight);
		current = current->next;
	}
	printf("\n");
}


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