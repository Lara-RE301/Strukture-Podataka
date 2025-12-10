#include "Structures.h"
#include "Push&Pop.h"

int RootDir(DirPosition dirHead, StackPosition stackHead) {

	DirPosition RootDir = NULL;
	RootDir = (DirPosition)malloc(sizeof(Directory));
	if (RootDir == NULL) {
		printf("Error: It was not possible to allocate the memory");
		return EXIT_FAILURE;
	}
	strcpy(RootDir->directory_name, "ROOT");
	RootDir->SubDirPosition = NULL;

	RootDir->Next = dirHead->Next;
	dirHead->Next = RootDir;

	Push(RootDir, stackHead);

	return EXIT_SUCCESS;
}

int DeleteDirectory(DirPosition root) {

	if (!root) { return 0; }

	DeleteDirectory(root->SubDirPosition);
	DeleteDirectory(root->Next);
	free(root);

	return EXIT_SUCCESS;
}

int DeleteStack(StackPosition headStack) {

	StackPosition temp = NULL;

	while (headStack->Next != NULL) { //petlja za osloboditi elementi iz stoga
		temp = headStack->Next;
		headStack->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	return EXIT_SUCCESS;
}