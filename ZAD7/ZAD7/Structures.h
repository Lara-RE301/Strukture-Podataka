#ifndef STRUCTURES_H
#define STRUCTURES_H
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 50
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Directory* DirPosition;
typedef struct Stack* StackPosition;

typedef struct Directory {
	char directory_name[MAX_SIZE];
	DirPosition Next;
	DirPosition SubDirPosition;
}Directory;
typedef struct Stack {
	StackPosition Next;
	DirPosition DirLevel;
}Stack;

int RootDir(DirPosition dirHead, StackPosition stackHead);
int DeleteDirectory(DirPosition headDir);
int DeleteStack(StackPosition headStack);

#endif