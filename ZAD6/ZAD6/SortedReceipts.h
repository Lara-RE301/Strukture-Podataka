#ifndef RECEIPT_SORTED_H
#define RECEIPT_SORTED_H

typedef struct Articals* ArticalsPosition;
typedef struct Receipt* ReceiptPosition;

typedef struct Dates {
	int year;
	int month;
	int day;
}Dates;

typedef struct Articals {
	char name[50];
	double quantity;
	double price;
	ArticalsPosition Next;
}Articals;

typedef struct Receipt {
	char recipt_name[50];
	Dates analyzedDates;
	ReceiptPosition Next;
	ArticalsPosition headArtical;
}Receipt;

int SortedReceipts(ReceiptPosition head, ReceiptPosition newReceipt);
int analyzedDates(const char* receipt_name, Dates* date);
int compareDates(const Dates* d1, const Dates* d2);
int PrintSortedReceipts(ReceiptPosition head);
#endif