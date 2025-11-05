/*5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LENGHT 200
#define ERROR 1;
typedef struct Postfix* Position;
typedef struct Postfix {
	double number;
	Position Next;
}Postfix;

char ReadFile(char* postfix, int);
int Separetor(char* postfix, Position);
int Push(double, Position);
double Pop(Position);
double Operations(double, double, char);
int DeleteStack(Position);

int main() {
	Postfix Head = { .number = 0, .Next = NULL };
	char postfix[MAX_LENGHT] = "";
	
	ReadFile(postfix, sizeof(postfix));
	
	printf("\n Postfix from the file: [%s]", postfix);

	Separetor(postfix, &Head);

	DeleteStack(&Head);
	
	return 0;
}

char ReadFile(char* postfix, int postfix_size) { 

	FILE* fp = fopen("postfiks.txt", "r");		//otvaranje datoteku i provjera
	if (fp == NULL) {
		printf("\n Error: It was not possible to open the file.\n");
		return EXIT_FAILURE;
	}

	if (fgets(postfix, postfix_size, fp) == NULL) {  //fgets uzmi sve znakovi u datoteci, ubaci u niz postfix
		printf("\n Error: It was not possible to read the postfix from the file"); //javi ako nije bilo moguci ucitati datoteku
		return EXIT_FAILURE;
	}

	fclose(fp);
	return EXIT_SUCCESS;
}
int Separetor(char* postfix, Position p) {

	char* token;
	char* endptr; //prekidac pretvorba string to double
	double operand1, operand2, result, final_result;
	
	token = strtok(postfix, " \n\t"); //podijeli string na razni djelovi
	while (token != NULL) { //ako token nije prazan lazi se u petlji

		double number = strtod(token, &endptr); //pretvorba string u double vrijednost

		if (*token != '\0' && *endptr == '\0') { //ako je token broj i endptr je '\0' ubaci se u push
			Push(number, p);
		}

		else if (strlen(token) == 1 && strchr("+-*/", token[0]) != NULL) { //ako je duzina tokena =1 i u token je ponaden operator ude se u else if 

			if (p->Next == NULL || p->Next->Next == NULL) { //provjera ako u stogu se nalazi dovoljno brojevi za operaciju
				fprintf(stderr, "\nErro, the is not enought operands for '%c'.\n", token[0]);
				return EXIT_FAILURE;
			}
			operand1 = Pop(p); //izbaciju se elemenati iz vrha stoga 
			operand2 = Pop(p);

			if (isnan(operand1) || isnan(operand2)) //ako neki od operandi nisu brojevi vrati se greska
				return EXIT_FAILURE;

			result = Operations(operand1, operand2, token[0]);

			if (isnan(result)) //provjera ako je rezultat broj
				return EXIT_FAILURE;

			Push(result, p); //ubacuje se rezultat u stogu
		}

		else {
			printf("\n Error. Unknown element in the postfix: %s\n", token); 
			return EXIT_FAILURE;
		}
		token = strtok(NULL, " \n\t"); //ocisti se token
	}
		if (p->Next != NULL && p->Next->Next == NULL) { //ispise se rezultat operacije
			final_result = Pop(p);
			printf("\nRezult is: %.2f\n", final_result);
			return EXIT_SUCCESS;
		}

		else if (p->Next != NULL) { //provjeri ako ima previse operacije
			printf(stderr, "\n Error: The expression has a previous operand.\n");
			return EXIT_FAILURE;
		}
		else { //provjera ako stog je prazan ili ne moze se racunati
			printf(stderr, "\n Error: Expression is empty or invalid.\n");
			return EXIT_FAILURE;
		}
	
	return EXIT_SUCCESS;
}

int Push(double number, Position Head) {

	Position newEl = NULL; //alociranje memoriju za element u stogu
	newEl = (Postfix*)malloc(sizeof(struct Postfix));
	if (newEl == NULL) {
		printf("\n Problem with th memory alocation");
		return EXIT_FAILURE;
	}

	newEl->number = number; //povezivanje elementi u stogu
	newEl->Next = Head->Next;
	Head->Next = newEl;

	return EXIT_SUCCESS;
}

double Pop(Position Head) {

	if (Head->Next == NULL) { //provjera ako je stog prazan
		printf("\n The Stack is epty");
		return NAN;
	}

	Position Gonna_Pop = Head->Next; //alociranje memorija za element koji ce biti "pop-an"
	double popped_data = Gonna_Pop->number;

	Head->Next = Gonna_Pop->Next; //brisanje koristeni elementi iz stoga
	Gonna_Pop->Next = NULL;
	free(Gonna_Pop);

	return popped_data;
}

double Operations(double operand1, double operand2, char chosenOperation) {
	double result = 0;

	switch (chosenOperation) { //switch case koji provjerava koja je poslana operacija i izracuna se rezultat
	case '+':
		result = operand1 + operand2;
		break;
	case '-':
		result = operand1 - operand2;
		break;
	case '*':
		result = operand1 * operand2;
		break;
	case'/':
		if (operand2 == 0.0) { //provjera da operand2 nije nula
			printf("\n It is not possible to divide with zero");
			return NAN;
		}
		result = operand1 / operand2;
		break;
	default:
		printf("\n Error. Unknown operation"); //na slucaj da se nade neku operaciju koja nije definirana
		return NAN;
	}
	return result;
}

int DeleteStack(Position Head) {
	Position temp = NULL;

	while (Head->Next != NULL) { //petlja za osloboditi elementi iz stoga
		temp = Head->Next;
		Head->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}

	printf("\n The stack was successfully freed");
	return 0;
}