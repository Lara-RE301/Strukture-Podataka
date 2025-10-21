/*Definirati strukturu osoba(ime, prezime, godina roðenja) i napisati program koji : A.dinamièki dodaje novi element na poèetak liste,
B.ispisuje listu,C.dinamièki dodaje novi element na kraj liste,D.pronalazi element u listi(po prezimenu),E.briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person* Position; //pokazivac na strukturu
typedef struct Person { 
	char name[50];
	char last_name[50];
	int birth_year;
	Position Next; //pokazivac na strukturu da se moze uhvatiti sljedeci element
}Person;

int addTotheBeginning(Position); //deklaracija svi funkcije koje su u ovom kodu
int Printlist(Position); 
int addTotheEnd(Position); 
Position FindlastName(char* lastName, Position); 
Position FindPreviouslastName(char* lastName, Position); 
int removeFromtheList(char* lastName, Position); 
int DeleteList(Position);

int main() {
	Person Head = { .name = "", .last_name = "", .birth_year = 0, .Next = NULL }; //postavljanje prvi element HEAD na "nulu", koji se nece ispisati
	char lastName[50];
	int temp = 0;
	Position personFound=NULL;
	
	addTotheBeginning(&Head); //pozivanje funkcija unos prije prethodni element
	printf("\n Lista nakon unosa na pocetku:\n");
	Printlist(Head.Next); //pozivanje funkcija za ispis

	addTotheEnd(&Head); // pozivanje funkcija za unos na kraju liste
	printf("\n Lista nakon unosa na kraju:\n");
	Printlist(Head.Next); //za ispis svi elementi

	printf("\n Zelite li pronaci odredenu osobu? Ako da, unesite prezime. Ako ne, unesite NE: "); // if/else uvjeti da korisnik moze odluciti ako zeli ili ne pronaci osobu iz liste.
	scanf("%s", lastName);																		
	if (strcmp(lastName, "NE") == 0) {															//ako odgovori ne, pojavi se poruku da nije htio,
		printf("\n Ne zelite nitko pronaci!");
	}
	else {
		personFound = FindlastName(lastName, &Head);											//ali ako unesi prezime, poziva se funkcija za pretragu koja vrati da je nasla ili nije nasla uneseni prezime
		if (personFound == NULL) {
			printf("\n Prezime koje ste unjeli nije u listi");
		}
		else
			printf("\n Prezime koje ste unjeli, %s, se nalazi u listi na adresu %p", lastName, personFound);
	}
		
	printf("\n Zelite li izbrisati netko iz liste? Ako da, unesite prezime. Ako ne, unesite NE: "); // if/else uvjeti ako korinik zeli izbrisati osobu iz liste
	scanf("%s", lastName);
	if (strcmp(lastName, "NE") == 0) {																//ako odgovor je ne, samo se pojavi poruka da nije htio
		printf("\n Ne zelite nitko izbrisati iz liste!");
	}
	else {
		temp = removeFromtheList(lastName, &Head);													//ako unesni prezime, sali se u funkciji removeFromtheList
		if (temp == EXIT_FAILURE) {																	//oviseci sto funkcija vraca, javit ce se greska ili da je uspjesno izbrisano
			printf("\n Doslo do greske. Nije bilo moguce izbrisati osobu iz liste");
		}
		else
			printf("\n Uspjesno ste obrisali osobu iz liste ");
	}

	Printlist(Head.Next);
	DeleteList(&Head);
	return 0;
}

int addTotheBeginning(Position p) {

	int N=0, i;
	printf("\n Koliko ljudi zelite unjeti u listi? ");
	scanf("%d", &N);

	for (i = 0; i < N; i++) {  //petlja za unos u listi
	//potrebno je da alociranje memoriju za q bude unutra petlje jer za svaku novu osobu stvorit ce se novi prostor u memoriji
		Position q;
		q = (Position)malloc(sizeof(Person));
		if (q == NULL) {
			printf("Greska pri alociranje memoriju");
			return EXIT_FAILURE;
		}
		printf("\nIme i prezime: "); //unos ime, prezime i godinu rodenje
		scanf("%s %s", q->name, q->last_name);
		printf("\nGodinu rodenje: ");
		scanf("%d", &q->birth_year);

		q->Next = p->Next; //povezivanje elementi iz liste korisceni pokazivaci
		p->Next = q;	
	}
	return 0;
}

int Printlist(Position p) {
	printf("\n Uneseni ljudi u listu:\n");
	printf("IME PRETIME GODINU RODENJE\n");
	while (p != NULL) { //while petlja za ispis elementi iz liste
		printf("\n%s %s %d", p->name, p->last_name, p->birth_year);
		puts("");
		p = p->Next; //prelazak na sljedeci element iz liste
	}
	return 0;
}

int addTotheEnd(Position p) {
		while (p->Next != NULL) { //petlja da se doðe do zadnjeg element listi
		p = p->Next;
		}

	addTotheBeginning(p); //pozivan funkciju addTotheBeginning poslajuæi zadnji element u listi
	return 0;
	}

Position FindlastName(char* lastName, Position p) {
	
	while (p != NULL && strcmp(p->last_name, lastName) != 0) {	//while petlja se vrti sve dok je p=NULL ili ako je se pronade poslan prezime
		p = p->Next;
	}
	return p;
}

Position FindPreviouslastName(char* lastName, Position p) {

	while (p->Next != NULL && strcmp(p->Next->last_name, lastName) != 0) { //while petlja pronaci koji je element prije od onaj koji smo poslali
		p = p->Next;
	}
	return p;
}
int removeFromtheList(char* lastName, Position p) {

	Position Previous = FindPreviouslastName(lastName, p); //alociranje memoriju za Previous i istovremeno poziva se funkcija za pretragu
	if (Previous->Next == NULL) return EXIT_FAILURE;

	Position temp = Previous->Next; //povezivanje prethodni element s element koji je nakon onaj koji zelimo izbrisati
	Previous->Next = temp->Next;
	free(temp);
	return 1;
}

int DeleteList(Position p) {
	Position Current = p;
	Position temp = 0;

	while (Current != NULL) { //petlja za isbrisanje listu, tj. oslobodi se memoriju u mallocu
		temp = Current->Next;
		free(Current);
		Current = temp;
	}
	return 0;
}
