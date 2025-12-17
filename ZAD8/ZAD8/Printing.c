#include "Printing.h"

int InOrder(TreePosition Root) {

	if (Root == NULL) {
		printf("There are no elements in the tree");
		return EXIT_SUCCESS;
	}

	InOrder(Root->Left);
	printf("%d ", Root->value);
	InOrder(Root->Right);

	return EXIT_SUCCESS;
}

int PostOrder(TreePosition Root) {

	if (Root == NULL) {
		printf("There are no elements in the tree");
		return EXIT_SUCCESS;
	}

	PostOrder(Root->Left);
	PostOrder(Root->Right);
	printf("%d ", Root->value);

	return EXIT_SUCCESS;
}

int PreOrder(TreePosition Root) {

	if (Root == NULL) {
		printf("There are no elements in the tree");
		return EXIT_SUCCESS;
	}

	printf("%d ", Root->value);
	PreOrder(Root->Left);
	PreOrder(Root->Right);

	return EXIT_SUCCESS;
}

int LevelOrder(TreePosition Root, StackPosition HeadStack) {
	if (Root == NULL) {
		printf("There are no elements in the tree");
		return EXIT_SUCCESS;
	}
	HeadStack->Next = NULL;
	Queue(HeadStack, Root);

	while (HeadStack->Next != NULL) {
		TreePosition Current = Dequeue(HeadStack);
		printf("%d ", Current->value);

		if (Current->Left) Queue(HeadStack, Current->Left);
		if (Current->Right) Queue(HeadStack, Current->Right);
	}
	return EXIT_SUCCESS;
}

StackPosition Queue(StackPosition HeadStack, TreePosition TreeNode) {

	if (TreeNode == NULL) { return HeadStack; }

	StackPosition StackNode = (StackPosition)malloc(sizeof(Stack));
	if (StackNode == NULL) {
		printf("Error: It was not possible to allocate the memory");
		return HeadStack;
	}
	StackNode->TreeLevel = TreeNode;
	StackNode->Next = NULL;

	StackPosition temp = HeadStack;
	while (temp->Next != NULL) {
		temp = temp->Next;
	}
	temp->Next = StackNode;

	return HeadStack; 
}

TreePosition Dequeue(StackPosition HeadStack) {
	if (HeadStack->Next == NULL) return NULL;

	StackPosition First = HeadStack->Next;
	TreePosition TreeLeaf = First->TreeLevel;

	HeadStack->Next = First->Next;
	free(First);

	return TreeLeaf;
}
