// For Visual Studios go to project properties -> Configuration Properties -> C/C++ -> General -> SDL checks -> No.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{	
	Node* left;
	Node* right;
	int eger;
	int null;
} Node;  
Node* add(Node* root, Node* newNode) {
	if (root->null) { // if I could just figure out a way to check for null pointers I could save so much memory
		root->eger = newNode->eger;
		root->left = newNode->left;
		root->right = newNode->right;
		root->null = newNode->null;
	}//root = newNode; is not equivalent to the above
	else // got this setup so smaller numbers go to the left and bigger to the right
		if (root->eger > newNode->eger)
			root->left = add(root->left, newNode);
		else // (->) makes c so convenient
			root->right = add(root->right, newNode);
	return root;
}
void addition(int value, Node* root) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->left = (Node*)malloc(sizeof(Node));
	newNode->right = (Node*)malloc(sizeof(Node));
	newNode->eger = value;
	newNode->null = 0;
	root = add(root, newNode);
}
int delBool;
Node* deletion(Node* root, int value) {
	if (root->null) // we do this so we don't compareTo with a null cuz thats bad
		return root; // also means we never found the thing we were supposed to subtract
	else
		if (root->eger > value)
			root->left = deletion(root->left, value);
		else if (root->eger < value)
			root->right = deletion(root->right, value);
		else {  // implicitly means root->eger is value ... we've found it
			if (root->left->null && root->right->null) {  // childfree case
				root->null++; // a null leaf has no use for leaves
				free(root->left); // delete the leaf
				free(root->right);// delete the leaf
			}
			else if (root->left->null && !root->right->null) { //left child case
				root = root->right; // switcharoo
				root->right->null++;// child replaces parent
			}
			else if (!root->left->null && root->right->null) { //right child case
				root = root->left; // switcharoo
				root->left->null++;// child replaces parent
			}
			else { // two children case 
				Node* current = root;
				while (!current->left->left->null) // find parent of minimal child of the greater path
					current = current->left;
				root->eger = current->left->eger; // replace value with that of minimal child so as to keep the root's node pointers
				current->left->null++; // mimimal child becomes the root in the subtree
			}
			delBool++;
		}
	return root; // this is the updated root after all the above recursion is done
}
int del(int value, Node* root) {
	delBool = 0;
	root = deletion(root, value);
	return delBool;
}
int discovery(int value, Node* root) {
	if (root->null || root->eger == value) 
		return root->null; // null is 0 if found and !0 if not found
	else
		if (root->eger > value) //tree issorted such that left contains the smaller and right the larger
			return discovery(value, root->left);
		else //(->) really declutters the code
			return discovery(value, root->right);
}
void showInOrder(Node* root) {
	if (root->null)
		return;
	showInOrder(root->left); // starts at left most leaf,then does parent, then leaf  to the right
	printf("%d, ", root->eger);
	showInOrder(root->right);
}
void delTree(Node* root) { //aka as postorder
	if (root->null) {
		free(root);
		return;
	}// does left most leaf then leaf to right of that then parent of both
	delTree(root->left);
	delTree(root->right);
	free(root);
}
int main(int argc, char *argv[])
{
	Node* root = (Node*)malloc(sizeof(Node));
	int run = 1;
	int found = 0;
	char choice = '\0';
	int input = 0;
	while (run) {
		if (choice != '\n')
			printf("This is a data structure that stores stuff.\n\tHit 'a' to add!\n\tHit 's' to show!\n\tHit 'd' to delete!\n\tHit 'f' to find!\n\tHit 'q to quit.\n");
		scanf("%1c", &choice);
		switch (choice) {
			case 'q':
				printf("You entered 'q' for quit.\tGood for you\n");//, &input);
				run = 0;
				delTree(root); // free the tree memory
				break;
			case 'a':
				printf("You entered 'a' for add.\tGood for you\n");//, &input);
				scanf("%d", &input);
				addition(input, root);
				break;
			case 's':
				printf("You entered 's' for show.\tGood for you\n");//, &input);
				showInOrder(root);
				break;
			case 'd':
				printf("You entered 'd' for delete.\tGood for you\n");//, &input);
				scanf("%d", &input);
				if (del(input, root))
					printf("%d was deleted.\tIsn't that wonderful?", input);
				else
					printf("No such %d was found.\tHonestly you should've known that.", input);
				break;
			case 'f':
				printf("You entered 'f' for find.\tGood for you\n");// , &input);
				scanf("%d", &input);
				if (!discovery(input, root))
					printf("You found it.\tThat's great I guess.\n");
				else 
					printf("You didn't find it.\tYou kinda suck at this.\n");
				break;
			case '\n': // Why do I need this?
				break;
			default:
				printf("Typing is hard.\tTry Again.\n");
				break;
		}	
	}
	return 0;
}
