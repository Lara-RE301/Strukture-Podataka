#ifndef PRINTING_H
#define PRINTING_H
#include "Structures.h"
#include <stdio.h>
#include <stdlib.h>

int InOrder(TreePosition Root);
int PostOrder(TreePosition Root);
int PreOrder(TreePosition Root);
int LevelOrder(TreePosition Root, StackPosition HeadStack);
StackPosition Queue(StackPosition HeadStack, TreePosition TreeNode);
TreePosition Dequeue(StackPosition HeadStack);

#endif