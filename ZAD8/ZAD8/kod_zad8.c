/*Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba
omoguæiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"
#include "Printing.h"
#define MAX_LENGHT 100

int UserInquery(TreePosition Root, StackPosition HeadStack);
TreePosition InsertInTree(TreePosition NewEl, int value);
TreePosition Search(TreePosition Root, int value);
TreePosition DeleteElement(TreePosition Root, int value);
TreePosition FindMin(TreePosition Root);
int DeleteTree(TreePosition Root);

int main() {
	TreePosition Root =NULL;
	Stack HeadStack = { .Next = NULL, .TreeLevel = NULL };
	
	UserInquery(Root, &HeadStack);
	
	return 0;
}

int UserInquery(TreePosition Root, StackPosition HeadStack) {

	int command=0, value=0;

	while (1) {
		printf("\n1. Insert element\n2. Delete element\n3. Find element\n4. Print (All forms)\n5. Exit\nSelection: ");
		if (scanf("%d", &command) != 1) {
			while (getchar() != '\n');
			continue;
		}

		switch (command) {
		case 1:
			printf("Enter a value: ");
			scanf("%d", &value);
			Root = InsertInTree(Root, value);
			break;
		case 2:
			printf("Enter the value you wish to delete: ");
			scanf("%d", &value);
			Root = DeleteElement(Root, value);
			break;
		case 3:
			printf("Enter a value to search: ");
			scanf("%d", &value);
			if (Search(Root, value))
				printf("Element %d is not in the tree", value);
			else 
				printf("Element was not found.");
			break;
		case 4:
			if (Root == NULL) {
				printf("\nThe tree is empty");
			}
			else {
				printf("\nInorder:   "); InOrder(Root);
				printf("\nPreorder:  "); PreOrder(Root);
				printf("\nPostorder: "); PostOrder(Root);
				printf("\nLevel:     "); LevelOrder(Root, HeadStack);
				printf("\n");
			}
			break;
		case 5:
			DeleteTree(Root);
			exit(0);
		default:
			printf("Invalid option");
		}
	}
	return EXIT_SUCCESS;
}

TreePosition InsertInTree(TreePosition Root, int value) {

	if (Root == NULL) {
		TreePosition NewEl = (TreePosition)malloc(sizeof(Tree));
		if (NewEl == NULL) {
			printf("Error: It was not possible to allocate the memory");
			return NULL;
		}
		NewEl->value = value;
		NewEl->Left = NULL; 
		NewEl->Right = NULL;
		return NewEl; 
	}

	if (Root !=NULL && value< Root->value) {
		Root->Left = InsertInTree(Root->Left, value);
	}
	else if (Root != NULL && value > Root->value) {
		Root->Right = InsertInTree(Root->Right, value);
	}
	return Root;
}

TreePosition Search(TreePosition Root, int value) {
	if (Root == NULL || Root->value == value) { return Root; }
	if (value < Root->value) { return Search(Root->Left, value); }
	return Search(Root->Right, value);
}

TreePosition FindMin(TreePosition Root) {
	if (Root == NULL)  return NULL; 
	while (Root->Left != NULL) {
		Root = Root->Left;
	}
	return Root;
}

TreePosition DeleteElement(TreePosition Root, int value) {

	if (Root == NULL) { 
		printf("Tree is empty");
		return NULL; 
	}

	if (value < Root->value) { 
		Root->Left = DeleteElement(Root->Left, value);
	}
	else if (value > Root->value) {
		Root->Right = DeleteElement(Root->Right, value);
	}

	else {
		if (Root->Left == NULL) {
			TreePosition temp = Root->Right;
			free(Root);
			return temp;
		}
		else if (Root->Right == NULL) {
			TreePosition temp = Root->Left;
			free(Root);
			return temp;
		}
		TreePosition temp = FindMin(Root->Right);
		Root->value = temp->value;
		Root->Right = DeleteElement(Root->Right, temp->value);
	}
	return Root;
}

int DeleteTree(TreePosition Root) {

	if (Root == NULL) return EXIT_SUCCESS;

	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	free(Root);
	return EXIT_SUCCESS;
}