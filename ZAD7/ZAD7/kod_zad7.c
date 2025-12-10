/*Napisati program koji pomoæu vezanih listi (stabala) predstavlja strukturu direktorija. Omoguæiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Toènije program treba preko menija simulirati korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"
#include "Push&Pop.h"
#define MAX_SIZE 50

int UserInquery(DirPosition dirHead, StackPosition stackHead);
int NewDirectory(char* name, StackPosition headStack);
int EnterDirectory(char* name, StackPosition headStack);
int PrintDirectory(DirPosition current, int level);

int main() {
	Directory HeadDir = { .directory_name = "", .Next = NULL, .SubDirPosition = NULL };
	Stack HeadStack = {.Next=NULL, .DirLevel=NULL};

	RootDir(&HeadDir, &HeadStack);
	UserInquery(&HeadDir, &HeadStack);
	DeleteDirectory(HeadDir.Next);
	DeleteStack(&HeadStack);

	return 0;
}

int UserInquery(DirPosition dirHead, StackPosition stackHead) {

	char command[MAX_SIZE];
	char* token1;
	char* token2;

	printf("</c>  ");
	if (fgets(command, sizeof(command), stdin) == NULL) {
		return EXIT_FAILURE;
	}
	command[strcspn(command, "\n")] = 0;
	token1 = strtok(command, " ");

	do {
		if (token1 == NULL) {
			printf("Command not found");
		}
		else if (strcmp(token1, "md") == 0) {
			token2 = strtok(NULL, " ");
			if (token2) { 
				NewDirectory(token2, stackHead);
			}
			else { printf("Missing directory name"); }
		}

		else if (strcmp(token1, "cd") == 0) {
			token2 = strtok(NULL, " ");
			if (token2) {
				if (strcmp(token2, "..") == 0) {
					if (Pop(stackHead)!= EXIT_SUCCESS) {
						printf("Already in the root directory");
					}
				}
				else {
					EnterDirectory(token2, stackHead);
				}
			}
			else {
				printf("Missing specification for cd");
			}
		}

		else if (strcmp(token1, "dir") == 0) {
			DirPosition current = stackHead->Next->DirLevel;
			if (current==NULL) { 
				printf("Error: directory not found or stack empty");
			}
			else {
				printf("\n---- Direcotry structure ---- \n");
				PrintDirectory(current, 1);
			}
		}

		else if (strcmp(token1, "izlaz") == 0) {
			break;
		}

		printf("</c>  ");
		if (fgets(command, sizeof(command), stdin) == NULL) {
			break;
		}
		command[strcspn(command, "\n")] = 0;
		token1 = strtok(command, " ");
	} while (1);
	
	return EXIT_SUCCESS;
}

int NewDirectory(char* name, StackPosition headStack ) {

	DirPosition ParentDir, NewDir = NULL;
	NewDir = (DirPosition)malloc(sizeof(Directory));
	if (NewDir == NULL) {
		printf("Error: It was not possible to allocate memory");
		return EXIT_FAILURE;
	}
	strcpy(NewDir->directory_name, name);
	NewDir->SubDirPosition = NULL;

	if (headStack->Next == NULL) { 
		printf("Error: The stack is empty");
		free(NewDir);
		return EXIT_FAILURE; 
	}
	
	ParentDir = headStack->Next->DirLevel;
	NewDir->Next = ParentDir->SubDirPosition;
	ParentDir->SubDirPosition = NewDir;

	return EXIT_SUCCESS;
}

int EnterDirectory(char* name, StackPosition headStack) {

	DirPosition temp = headStack->Next->DirLevel->SubDirPosition;
	while (temp!=NULL && strcmp(temp->directory_name, name) != 0) {
		temp = temp->Next;
	}
	if (temp != NULL) {
		Push(temp, headStack);
	}
	else printf("Directory not found");

	return EXIT_SUCCESS;
}

int PrintDirectory(DirPosition current, int level) {

	int i;
	DirPosition Sub = NULL;
	if (current == NULL) { return EXIT_FAILURE; }

	for (i = 0; i < level - 1; i++) {
		printf("|	");
	}
	printf("---- %s ----\n", current->directory_name);

	Sub = current->SubDirPosition;
	while (Sub != NULL) {
		if (Sub != NULL) {
			PrintDirectory(Sub, level + 1);
			Sub = current->SubDirPosition;
		}
			Sub = Sub->Next;
	}
	
	return EXIT_SUCCESS;
}