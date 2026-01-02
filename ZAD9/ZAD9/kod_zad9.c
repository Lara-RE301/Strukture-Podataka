#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Tree* TreePosition;
typedef struct Stack* StackPosition;
typedef struct Tree {
	int value;
	TreePosition Left;
	TreePosition Right;
}Tree;

TreePosition InsertInTree(TreePosition NewEl, int value);
TreePosition MemoryAllocation(int value);
int HeadOfTheOperation(TreePosition Root);
int ReplaceElement(TreePosition Root);
int DeleteTree(TreePosition Root);
int InOrder(TreePosition Root);
int PrintInFile(TreePosition Root, FILE* fp);
TreePosition GenerateRandomTree();

int main() {
	srand((unsigned)time(NULL));
	TreePosition Root = NULL;

	HeadOfTheOperation(Root);

	return 0;
}
int HeadOfTheOperation(TreePosition Root) {

	FILE* fp = fopen("Trees.txt", "w");
	if (fp == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
	}

	int value[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };

	for (int i = 0; i < 10; i++) {
		Root = InsertInTree(Root, value[i]);
	}

	printf("\n----Inoder Tree 1----\n");
	InOrder(Root);
	PrintInFile(Root, fp);

	ReplaceElement(Root);

	printf("\n----Inorder Tree 2----\n");
	InOrder(Root);
	fprintf(fp, "\n");
	PrintInFile(Root, fp);


	DeleteTree(Root);
	Root = NULL;

	Root = GenerateRandomTree();
	printf("\n----InOder Tree 3----\n");
	InOrder(Root);
	fprintf(fp, "\n");
	PrintInFile(Root, fp);

	DeleteTree(Root);
	fclose(fp);
	return 0;
}

TreePosition InsertInTree(TreePosition Root, int value) {

	if (Root == NULL) {
		return MemoryAllocation(value);
	}

	if (value >= Root->value) {
		Root->Left = InsertInTree(Root->Left, value);
	}
	else {
		Root->Right = InsertInTree(Root->Right, value);
	}
	return Root;
}

TreePosition MemoryAllocation(int value) {

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

int ReplaceElement(TreePosition Root) {

	if (Root == NULL)  return 0;
	int PreValue = Root->value;

	Root->value = ReplaceElement(Root->Left) + ReplaceElement(Root->Right);

	return Root->value + PreValue;
}

TreePosition GenerateRandomTree() {
	int i = 0, number_random = 0;
	TreePosition Root = NULL;
	for (i = 0; i < 10; i++) {
		number_random = (rand() % 79) + 11;
		Root = InsertInTree(Root, number_random);
	}
	return Root;
}

int InOrder(TreePosition Root) {

	if (Root == NULL) { return EXIT_SUCCESS; }

	InOrder(Root->Left);
	printf("%d ", Root->value);
	InOrder(Root->Right);

	return EXIT_SUCCESS;
}

int PrintInFile(TreePosition Root, FILE* fp) {
	
	if (Root == NULL) { return 0; }
	PrintInFile(Root->Left, fp);
	fprintf(fp, "%d ", Root->value);
	PrintInFile(Root->Right, fp);

	return 0;
}

int DeleteTree(TreePosition Root) {

	if (Root == NULL) return EXIT_SUCCESS;

	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	free(Root);
	return EXIT_SUCCESS;
}