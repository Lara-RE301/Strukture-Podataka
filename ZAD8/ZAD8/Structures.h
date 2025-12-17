#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Tree* TreePosition;
typedef struct Stack* StackPosition;
typedef struct Tree {
	int value;
	TreePosition Left;
	TreePosition Right;
}Tree;

typedef struct Stack {
	StackPosition Next;
	TreePosition TreeLevel;
}Stack;

#endif 
