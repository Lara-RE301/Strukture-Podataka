#define _CRT_SECURE_NO_WARNINGS
#include "DeleteLists.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int DeleteReceiptList(ReceiptPosition head) {
	ReceiptPosition temp = NULL;

	while (head->Next != NULL) { //petlja za osloboditi elementi iz liste
		temp = head->Next;
		head->Next = temp->Next;
		if (temp->headArtical != NULL) { DeleteArticalList(temp->headArtical); }
		temp->Next = NULL;
		free(temp);
	}
	return EXIT_SUCCESS;
}

int DeleteArticalList(ArticalsPosition head) {
	ArticalsPosition temp = NULL;

	while (head->Next != NULL) { //petlja za osloboditi elementi iz liste
		temp = head->Next;
		head->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	free(head);
	return EXIT_SUCCESS;
}
