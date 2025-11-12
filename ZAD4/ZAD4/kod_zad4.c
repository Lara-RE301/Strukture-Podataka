#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LENGHT 100
typedef struct Polinom* Position;
typedef struct Polinom {
	int coefficient;
	int power;
	Position Next;
}Polinom;

int OpenReadFirstFile(Position);
int OpenReadSecondFile(Position);
int SortedEntry(Position, Position);
Position AllocateMemory(int, int);
int Addition(Position Head, Position FirstPoly, Position SecondPoly);
int Multiplication(Position Head, Position FirstPoly, Position SecondPoly);
int DeleteFromhead(Position Head);
int PrintPolinom(Position);
int DeleteEverything(Position);

int main() {
	Polinom Head = { .coefficient = 0, .power = 0, .Next = NULL };
	Polinom firstPolinom = { .coefficient = 0, .power = 0, .Next = NULL };
	Polinom secondPolinom = { .coefficient = 0, .power = 0, .Next = NULL };

	OpenReadFirstFile(&firstPolinom);
	OpenReadSecondFile(&secondPolinom);

	printf("\n-----First Polinom-----\n");
	PrintPolinom(&firstPolinom);
	printf("\n-----Second Polinom-----\n");
	PrintPolinom(&secondPolinom);

	Addition(&Head, firstPolinom.Next, secondPolinom.Next);
	printf("\n-----Addition Polinoms-----\n");
	PrintPolinom(&Head);

	Multiplication(&Head, firstPolinom.Next, secondPolinom.Next);
	printf("\n-----Nultiplication Polinoms-----\n");
	PrintPolinom(&Head);

	DeleteEverything(&Head);
	DeleteEverything(&firstPolinom);
	DeleteEverything(&secondPolinom);
	return 0;
}

int OpenReadFirstFile(Position firstpolinom) {
	int buffer_coefficient = 0, buffer_power = 0;
	Position NewEl;
	FILE *fp = fopen("first_polinom.txt", "r");
	if (fp == NULL) {
		printf("\n Error: It was not possible to open the file!");
		return EXIT_FAILURE;
	}

	while (fscanf(fp, "%d %d", &buffer_coefficient, &buffer_power) == 2) {
		NewEl=AllocateMemory(buffer_coefficient, buffer_power);
		SortedEntry(NewEl, firstpolinom);
	}
	
	fclose(fp);
	return EXIT_SUCCESS;
}

int OpenReadSecondFile(Position secondpolinom) {
	int buffer_coefficient = 0, buffer_power = 0;
	Position NewEl;
	FILE* fp = fopen("second_polinom.txt", "r");
	if (fp == NULL) {
		printf("\n Error: It was not possible to open the file!");
		return EXIT_FAILURE;
	}

	while (fscanf(fp, "%d %d", &buffer_coefficient, &buffer_power) == 2) {
		NewEl = AllocateMemory(buffer_coefficient, buffer_power);
		SortedEntry(NewEl, secondpolinom);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

Position AllocateMemory(int coefficient, int power) {
	Position NewEl;
	NewEl = (Position)malloc(sizeof(Polinom));
	if (NewEl == NULL) {
		printf("\n Error: It was not possible to alocate the memory");
		return NULL;
	}
	NewEl->coefficient = coefficient;
	NewEl->power = power;
	NewEl->Next = NULL;

	return NewEl;
}

int SortedEntry(Position NewEl, Position Head) {
	Position PreviousEl = Head, q= Head->Next;

	if (q == NULL) {
		NewEl->Next = PreviousEl->Next;
		PreviousEl->Next = NewEl;
		return EXIT_SUCCESS;
	}
	while (q!=NULL && q->power > NewEl->power) {
		PreviousEl = q;
		q = q->Next;
		}
	if (q != NULL && q->power == NewEl->power) {
		q->coefficient += NewEl->coefficient;
		if (q->coefficient == 0) {
			DeleteFromhead(Head);
		}
		NewEl->Next = NULL;
		free(NewEl);
		return EXIT_SUCCESS;
	}
	else{
		NewEl->Next = q;
		PreviousEl->Next = NewEl;
		}
	return EXIT_SUCCESS;
}

int Addition(Position Head, Position FirstPoly, Position SecondPoly) {
	Position q = FirstPoly, p = SecondPoly;
	Position NewEl;

	while (q != NULL) {
		NewEl = AllocateMemory(q->coefficient, q->power);
		if (NewEl == NULL) return EXIT_FAILURE;
		SortedEntry(NewEl, Head);
		q = q->Next;
	}
	while (p != NULL) {
		NewEl = AllocateMemory(p->coefficient, p->power);
		if (NewEl == NULL) return EXIT_FAILURE;
		SortedEntry(NewEl, Head);
		p = p->Next;
	}
	return EXIT_SUCCESS;
}

int Multiplication(Position Head, Position FirstPoly, Position SecondPoly) {
	Position temp2 = SecondPoly;
	Position NewEl;
	int power, coefficient;

	while (FirstPoly != NULL) {
		SecondPoly = temp2;
		while (SecondPoly != NULL) {
			coefficient = FirstPoly->coefficient * SecondPoly->coefficient;
			power = FirstPoly->power + SecondPoly->power;
			NewEl = AllocateMemory(coefficient, power);

			SortedEntry(NewEl, Head);
			SecondPoly=SecondPoly->Next;
		}
		FirstPoly = FirstPoly->Next;
	}
	DeleteFromhead(Head);
	return EXIT_SUCCESS;
}

int PrintPolinom(Position Head) {
	Position current = Head->Next;
	int flag = 1;
	int abs_coeff;

	if (current == NULL) {
		printf("\nIt is empty!");
		return EXIT_SUCCESS;
	}
	while (current != NULL) {
		if (current->coefficient > 0 && !flag) {
			printf(" + ");
		}
		else if (current->coefficient < 0) {
			printf(" - ");
		}
		abs_coeff = abs(current->coefficient);
		if (abs_coeff != 1 || current->power == 0) {
			printf("%d", abs_coeff);
		}
		if (current->power > 1) {
			printf("x^%d", current->power);
		}
		else if (current->power == 1) {
			printf("x");
		}
			flag = 0;
			current = current->Next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int DeleteFromhead(Position Head) {
	Position Previous = Head;
	Position temp = Head->Next;
	while (temp != NULL) {
		if (temp->coefficient == 0) {
			Previous->Next = temp->Next;
			temp->Next = NULL;
			free(temp);
			temp = Previous->Next;
		}
		else {
			Previous = temp;
			temp = temp->Next;
		}
	}
	return EXIT_SUCCESS;
}

int DeleteEverything(Position p) {
	Position temp = 0;
	while (p->Next != NULL) { //petlja za isbrisanje listu, tj. oslobodi se memoriju u mallocu
		temp = p->Next;
		p->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	return 0;
}