/*Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor vezane liste sadržava vezanu listu 
artikala sortiranu po nazivu artikla. Nakon toga potrebno je omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SortedReceipts.h"
#include "DeleteLists.h"
#include "User_Inquery.h"
#define MAX_LENGHT 50

int ReadFile(ReceiptPosition head);
ReceiptPosition readReceipt(char*, ReceiptPosition head);
int ReadArticals(FILE* fp, ReceiptPosition newReceipt, const char* filename);
int SortArticals(ArticalsPosition head, ArticalsPosition newArtical);


int main() {
	Receipt Head = { .recipt_name = "", .Next = NULL, .headArtical =NULL};
	
	ReadFile(&Head);
	PrintSortedReceipts(&Head);
	UserInquery(&Head);
	DeleteReceiptList(&Head);
	
	return 0;
}

int ReadFile(ReceiptPosition head) {
	
	char filename[MAX_LENGHT];
	FILE* fp = fopen("racuni.txt", "r"); //otvaranje i provjera datoteke
	if (fp == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
	}

	while (fgets(filename, MAX_LENGHT, fp)!=NULL) { //iscita se ime racuni iz datoteke
		filename[strcspn(filename, "\n")] = 0; //makni se "\n" iz stringa
		ReceiptPosition newReceipt = readReceipt(filename, head); //sali se ime racun na funkciju koja ce citati artikli iz racuna

		if (newReceipt != NULL) { //provjera ako nije prazan i onda se sali racun na sortiranje
			SortedReceipts(head, newReceipt); //nakon sto se odradi readReceipt sali se head i taj novi receipt u funkciji (u header file SortedReceipts)
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

	ReceiptPosition newReceipt = (ReceiptPosition)malloc(sizeof(Receipt)); //alociranje memorija za racun i artikli
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

	strcpy(HeadArticalNode->name, ""); //inicijalizacija struktura za artikli
	HeadArticalNode->quantity = 0.0;
	HeadArticalNode->price = 0.0;
	HeadArticalNode->Next = NULL;
	newReceipt->headArtical = HeadArticalNode;

	if (fgets(buffer_date, MAX_LENGHT, fp) != NULL) { //buffer koji sacuva datum racun
		buffer_date[strcspn(buffer_date, "\n")] = 0;
	
		strcpy(newReceipt->recipt_name, buffer_date); //prepisivanje informacija iz buffer u strukturi Receipts
	}

	else {
		printf("Error: It was not possible to read date from file");
		free(HeadArticalNode); free(newReceipt); fclose(fp);
		return NULL;
	}

	if (ReadArticals(fp, newReceipt, filename) != EXIT_SUCCESS) { //pokazivac na file, struktura racun i njegov ime
		fclose(fp);
		return NULL;
	}
	return newReceipt;
}

int ReadArticals(FILE* fp, ReceiptPosition newReceipt, const char* filename) {

	char buffer [MAX_LENGHT];
	char name_buffer[MAX_LENGHT];
	double elements = 0, price, quantity;

	if (fp == NULL || newReceipt == NULL) { return EXIT_FAILURE; }

	while (fgets(buffer, MAX_LENGHT, fp) != NULL) { //provjera i unos u buffer
		buffer[strcspn(buffer, "\n")] = 0;

		ArticalsPosition newArtical = (ArticalsPosition)malloc(sizeof(Articals));
		if (newArtical == NULL) {
			printf("Error: It was not possible to allocate the memory");
			return EXIT_FAILURE;
		}
		newArtical->Next = NULL;

		elements = sscanf(buffer, "%[^,], %lf, %lf", name_buffer, &quantity, &price); //isictavanje elementi iz racun u name_buffer i varijabli
		if (elements == 3) {
			name_buffer[strcspn(name_buffer, "\n")] = 0;

			strcpy(newArtical->name, name_buffer); //iz buffer prebacuje se u strukturi
			newArtical->quantity= quantity;
			newArtical->price = price;

			SortArticals(newReceipt->headArtical, newArtical); //sali se da se artikli poredaju po ime
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

	while (q->Next != NULL && strcmp(newArtical->name, q->Next->name) > 0) { //sortiranje artikli po ime
		q = q->Next;
	}
	newArtical->Next = q->Next;
	q->Next = newArtical;

	return EXIT_SUCCESS;
}