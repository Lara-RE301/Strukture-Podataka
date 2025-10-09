#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BOD 100

typedef struct {

	char ime[20];
	char prezime[20];
	double bodovi;
}Studenti;

int main() {

	int i, br_red = 0;
	char znak;
	double relativan_br_bod = 0;
	Studenti* student;
	FILE* fp = fopen("Popis_studenti.txt", "r");
	if (fp == NULL) {
		printf("\n Greska pri otvaranje datoteke!!");
		return 1;
	}
	
	while ((znak = fgetc(fp)) != EOF) {
		if (znak == '\n') {
			br_red++;
		}
	}
	rewind(fp);

	student = (Studenti*)malloc(br_red * sizeof(Studenti));
	if (student == NULL) {
		printf("Greska pri alociranje memorija!!");
		return 1;
	}

	for (i = 0; i < br_red; i++) {
		fscanf(fp,"%s %s %lf", student[i].ime, student[i].prezime, &student[i].bodovi);
	}
	printf("\nIME:	PREZIME:	BODOVI:		POSTOTAK:\n");
	for (i = 0; i < br_red; i++) {
		relativan_br_bod = (student[i].bodovi / MAX_BOD) * 100;
		printf("%s	%s		%.2lf		%.2lf", student[i].ime, student[i].prezime, student[i].bodovi, relativan_br_bod);
		puts(" ");
	}
	return 0;
}