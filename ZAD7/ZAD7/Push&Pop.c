#include "Push&Pop.h"

int Push(DirPosition El, StackPosition stackHead) {

	StackPosition newDir = NULL; //alociranje memoriju za element u stogu
	newDir = (StackPosition)malloc(sizeof(struct Stack));
	if (newDir == NULL) {
		printf("\n Problem with the memory alocation");
		return EXIT_FAILURE;
	}

	newDir->DirLevel = El;
	newDir->Next = stackHead->Next;
	stackHead->Next = newDir;

	return EXIT_SUCCESS;
}

int Pop(StackPosition stackHead) {

	if (stackHead->Next == NULL || stackHead->Next->Next == NULL) { //provjera ako je stog prazan
		return EXIT_FAILURE;
	}

	StackPosition Gonna_Pop = stackHead->Next; //alociranje memorija za element koji ce biti "pop-an"
	stackHead->Next = Gonna_Pop->Next; //brisanje koristeni elementi iz stoga
	Gonna_Pop->Next = NULL;
	free(Gonna_Pop);

	return EXIT_SUCCESS;
}