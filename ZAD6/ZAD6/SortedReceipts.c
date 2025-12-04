#define _CRT_SECURE_NO_WARNINGS
#include "SortedReceipts.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int SortedReceipts(ReceiptPosition head, ReceiptPosition newReceipt) { //funkcija za sortiranje po datum

	ReceiptPosition q=head;
	if (head == NULL || newReceipt == NULL) {
		return EXIT_FAILURE;
	}

	while (q->Next != NULL && strcmp(newReceipt->recipt_name, q->Next->recipt_name) > 0) {
		q = q->Next;
	}

	newReceipt->Next = q->Next;
	q->Next = newReceipt;

	return EXIT_SUCCESS;
}

int PrintSortedReceipts(ReceiptPosition head) { //ispisivanje sortirani racuni
	ReceiptPosition p = head->Next;
	int i = 1;
	printf("\n----Sorted receipts----\n");
	while (p != NULL) {
		printf("     (%d)%s \n", i, p->recipt_name);
		p = p->Next;
		i++;
	}
	puts("");
	return EXIT_SUCCESS;
}