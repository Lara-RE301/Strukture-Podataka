#ifndef RECEIPT_SORTED_H
#define RECEIPT_SORTED_H

typedef struct Articals* ArticalsPosition;
typedef struct Receipt* ReceiptPosition;

typedef struct Articals {
	char name[50];
	double quantity;
	double price;
	ArticalsPosition Next;
}Articals;

typedef struct Receipt {
	char recipt_name[50];
	ReceiptPosition Next;
	ArticalsPosition headArtical;
}Receipt;

int SortedReceipts(ReceiptPosition head, ReceiptPosition newReceipt);
int PrintSortedReceipts(ReceiptPosition head);
#endif