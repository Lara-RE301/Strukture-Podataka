/*Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor vezane liste sadržava vezanu listu 
artikala sortiranu po nazivu artikla. Nakon toga potrebno je omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "SortedReceipts.h"

#define MAX_LENGHT 50

int ReadFile(ReceiptPosition head);
ReceiptPosition readReceipt(char*, ReceiptPosition head);
int ReadArticals(FILE* fp, ReceiptPosition newReceipt, const char* filename);
int SortArticals(ArticalsPosition head, ArticalsPosition newArtical);
int UserInquery(ReceiptPosition head);
int DeleteArticalList(ArticalsPosition head);
int DeleteReceiptList(ReceiptPosition head);

int main() {
	Receipt Head = { .recipt_name = "", .Next = NULL, .headArtical =NULL};
	
	ReadFile(&Head);
	UserInquery(&Head);
	DeleteReceiptList(&Head);

	return 0;
}

int ReadFile(ReceiptPosition head) {
	
	char filename[MAX_LENGHT];
	FILE* fp = fopen("racuni.txt", "r");
	if (fp == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
	}

	while (fgets(filename, MAX_LENGHT, fp)!=NULL) {
		filename[strcspn(filename, "\n")] = 0;
		ReceiptPosition newReceipt = readReceipt(filename, head);

		if (newReceipt != NULL) {
			SortedReceipts(head, newReceipt);
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

ReceiptPosition readReceipt(const char* filename, ReceiptPosition head) {

	char buffer_date[MAX_LENGHT];
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error: It was not possible to open the file");
		return NULL;
	}

	ReceiptPosition newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (newReceipt == NULL) {
		printf("Error: It was not possible to allocate the memory");
		fclose(fp);
		return NULL;
	}
	newReceipt->Next = NULL;

	ArticalsPosition HeadArticalNode = (ArticalsPosition)malloc(sizeof(Articals));
	if (HeadArticalNode == NULL) {
		printf("Error: It was not possible to allocate the memory");
		fclose(fp);
		return NULL;
	}

	strcpy(HeadArticalNode->name, "");
	HeadArticalNode->quantity = 0.0;
	HeadArticalNode->price = 0.0;
	HeadArticalNode->Next = NULL;
	newReceipt->headArtical = HeadArticalNode;

	if (fgets(buffer_date, MAX_LENGHT, fp) != NULL) {
		int lenght = strlen(buffer_date);
		while (lenght > 0 && isspace((unsigned char)buffer_date[lenght - 1])) { lenght--; }
		buffer_date[lenght]= '\0';

		if (lenght!=10) {
			printf("Error: Date does not have the correct lenght in file %s\n", filename);
			free(HeadArticalNode);
			free(newReceipt);
			fclose(fp);
			return NULL;
		}

		strcpy(newReceipt->recipt_name, buffer_date);

		if (analyzedDates(newReceipt->recipt_name, &newReceipt->analyzedDates) != EXIT_SUCCESS) {
			printf("Error: Invalid date format in file %s\n", filename);
			free(HeadArticalNode); 
			free(newReceipt); 
			fclose(fp); 
			return NULL;
		}
	}

	else {
		printf("Error: It was not possible to read date from file");
		free(HeadArticalNode);
		free(newReceipt);
		fclose(fp);
		return NULL;
	}

	if (ReadArticals(fp, newReceipt, filename) != EXIT_SUCCESS) {
		fclose(fp);
		return NULL;
	}
	return newReceipt;
}

int ReadArticals(FILE* fp, ReceiptPosition newReceipt, const char* filename) {

	char buffer [MAX_LENGHT];
	char name_buffer[MAX_LENGHT];
	double elements = 0, price, quantity;

	if (fp == NULL || newReceipt == NULL) return EXIT_FAILURE;

	while (fgets(buffer, MAX_LENGHT, fp) != NULL) {
		buffer[strcspn(buffer, "\n")] = 0;

		ArticalsPosition newArtical = (ArticalsPosition)malloc(sizeof(Articals));
		if (newArtical == NULL) {
			printf("Error: It was not possible to allocate the memory");
			return EXIT_FAILURE;
		}
		newArtical->Next = NULL;

		elements = sscanf(buffer, "%[^,], %lf, %lf", name_buffer, &quantity, &price);
		if (elements == 3) {
			int lenght = strlen(name_buffer);
			while (lenght > 0 && isspace((unsigned char)name_buffer[lenght - 1]))
			{
				name_buffer[--lenght] = '\0';
			}

			strcpy(newArtical->name, name_buffer);
			newArtical->quantity= quantity;
			newArtical->price = price;

			SortArticals(newReceipt->headArtical, newArtical);
		}
		else {
			printf("Error: Invalid information");
			free(newArtical);
		}
	}
	return EXIT_SUCCESS;
}

int SortArticals(ArticalsPosition head, ArticalsPosition newArtical) {
	
	if (head == NULL || newArtical == NULL)	return EXIT_FAILURE;

	ArticalsPosition q = head;

	while (q->Next != NULL && strcmp(newArtical->name, q->Next->name) > 0) {
		q = q->Next;
	}
	newArtical->Next = q->Next;
	q->Next = newArtical;

	return EXIT_SUCCESS;
}

int UserInquery(ReceiptPosition head) {

	ReceiptPosition q = head->Next;
	char artical[MAX_LENGHT];
	char beginning[12], ending[12];
	double total_quantity = 0.0, total_price = 0.0;
	Dates start, end;

	printf("What artical you want to check? ");
	if (fgets(artical, MAX_LENGHT, stdin) == NULL) { return EXIT_FAILURE; }
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
