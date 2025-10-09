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

int brojacStudenti() {

	FILE* fp = fopen("Popis_studenti.txt", "r");
	int br_red = 0;
	char znak;
	if (fp == NULL) {
		printf("\n Greska pri otvaranje datoteke!!");
		return 1;
	}

	while ((znak = fgetc(fp)) != EOF) {
		if (znak == '\n') {
			br_red++;
		}
	}
	fclose(fp);
	return br_red;
}

int IspisStudenti(int broj_student, Studenti* student) {
	int i;
	double relativan_br_bod = 0;
	printf("\nIME i PREZIME:		BODOVI:		RELATIVAN (postotak):\n");
	for (i = 0; i < broj_student; i++) {
		relativan_br_bod = (student[i].bodovi / MAX_BOD) * 100;
		printf("%s %s		%.2lf		%.2lf", student[i].ime, student[i].prezime, student[i].bodovi, relativan_br_bod);
		puts(" ");
	}
	return 0;
}

int main() {

	int i, broj_student;
	
	broj_student = brojacStudenti();
	Studenti* student;

	FILE* fp = fopen("Popis_studenti.txt", "r");
	
	student = (Studenti*)malloc(broj_student * sizeof(Studenti));
	if (student == NULL) {
		printf("Greska pri alociranje memorija!!");
		return 1;
	}

	for (i = 0; i < broj_student; i++) {
		fscanf(fp,"%s %s %lf", student[i].ime, student[i].prezime, &student[i].bodovi);
	}
	
	IspisStudenti(broj_student, student);

	fclose(fp);
	free(student);
	return 0;
}