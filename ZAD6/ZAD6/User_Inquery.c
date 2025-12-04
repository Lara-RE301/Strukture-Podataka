#define _CRT_SECURE_NO_WARNINGS
#include "User_Inquery.h"
#include <string.h>
#include <ctype.h>

int UserInquery(ReceiptPosition head) {

	ReceiptPosition q = head->Next;
	char artical[50];
	char beginning[12], ending[12];
	double total_quantity = 0.0, total_price = 0.0;
	Dates start, end;

	printf("\n----User Inquery----\n");
	printf("What artical you want to check? ");
	if (fgets(artical, 50, stdin) == NULL) { return EXIT_FAILURE; }
	artical[strcspn(artical, "\n")] = 0;

	printf("\n Enter which date to start the search (YYYY-MM-DD): ");
	if (fgets(beginning, 12, stdin) == NULL) { return EXIT_FAILURE; }

	beginning[strcspn(beginning, "\n")] = 0;
	if (analyzedDates(beginning, &start) != 0) {
		printf("\nError: The format is not correct"); return EXIT_FAILURE;
	}

	printf("\n Enter which date to end the search (YYYY-MM-DD): ");
	if (fgets(ending, 12, stdin) == NULL) { return EXIT_FAILURE; }
	ending[strcspn(ending, "\n")] = 0;
	if (analyzedDates(ending, &end) != EXIT_SUCCESS) {
		printf("\nError: The format is not correct"); return EXIT_FAILURE;
	}

	while (q != NULL) {
		if (compareDates(&q->analyzedDates, &start) >= 0 && compareDates(&q->analyzedDates, &end) <= 0) {
			ArticalsPosition p = q->headArtical->Next;
			while (p != NULL) {
				if (strcmp(p->name, artical) == 0) {
					total_quantity += p->quantity;
					total_price += (p->quantity * p->price);
				}
				p = p->Next;
			}
		}
		q = q->Next;
	}

	printf("\n----Choosen artikal %s and its results----\n", artical);
	if (total_quantity > 0) {
		printf("\n The amount bought: %.2lf\n", total_quantity);
		printf("\n The total price: %.2lf\n", total_price);
	}
	else {
		printf("The artikal was not bought in the entered time-space");
	}
	return EXIT_SUCCESS;
}