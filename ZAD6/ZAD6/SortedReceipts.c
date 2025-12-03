#define _CRT_SECURE_NO_WARNINGS
#include "SortedReceipts.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int analyzedDates(const char* receipt_name, Dates* date) {
	if (sscanf(receipt_name, "%d-%d-%d", &date->year, &date->month, &date->day) == 3) { return EXIT_SUCCESS; }
	else { return EXIT_FAILURE; }
}

int compareDates(const Dates* d1, const Dates* d2) {
	if (d1->year != d2->year) { return d1->year - d2->year; }
	if (d1->month != d2->month) { return d1->month - d2->month; }
	else { return d1->day - d2->day; }
}

int SortedReceipts(ReceiptPosition head, ReceiptPosition newReceipt) {

	ReceiptPosition q=head;
	if (head == NULL || newReceipt == NULL) {
		return EXIT_FAILURE;
	}

	while (q->Next != NULL && compareDates(&newReceipt->analyzedDates, &q->Next->analyzedDates)>0) {
		q = q->Next;
	}

	newReceipt->Next = q->Next;
	q->Next = newReceipt;

	return EXIT_SUCCESS;
}