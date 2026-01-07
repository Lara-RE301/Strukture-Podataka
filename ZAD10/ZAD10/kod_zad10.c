#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGHT 100

typedef struct Country* CountryPosition;
typedef struct City* CityPosition;
typedef struct Country {
	char country_name[50];
	CountryPosition Next;
	CityPosition Root;
}Country;
typedef struct City {
	char city_name[50];
	int population;
	CityPosition Left;
	CityPosition Right;
}City;

int ReadFromFile(CountryPosition Head);
int SortCountry(CountryPosition Head, char* country_name, CityPosition Root_City);
CityPosition InsertInTree(CityPosition Root, char* city_name, int populus);
CountryPosition AllocateMemoryCountry(char* name, CityPosition Root_City);
CityPosition AllocateMemoryCity(char* name, int populus );
int PrintCountries(CountryPosition Head);
int PrintCities(CityPosition Root);
int DeleteList(CountryPosition Head);
CityPosition DeleteTree(CityPosition Root);
int UserInquery(CountryPosition Head);
int FindCities(CityPosition Root, int populus);

int main() {
	Country Head = { .country_name = "", .Next = NULL, .Root=NULL };

	if (ReadFromFile(&Head) == EXIT_SUCCESS) {
		printf("\n----All countries and cities in the files----\n");
		PrintCountries(Head.Next);
		printf("----User Inquery----\n");
		UserInquery(Head.Next);
	}

	DeleteList(&Head);
	return 0;
}

int ReadFromFile(CountryPosition Head) {
	char buffer[MAX_LENGHT];
	char* token1=NULL;
	char* token2=NULL;

	FILE* fp_countries = fopen("drzave.txt", "r");
	if (fp_countries == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
	}
	
	while (fgets(buffer, MAX_LENGHT, fp_countries)!=NULL) {
		buffer[strcspn(buffer, "\n")] = 0;

		token1 = strtok(buffer, " ");
		token2 = strtok(NULL, " ");

		if (token1 && token2) { 
			CityPosition Root_City = NULL;

			FILE* fp_cities = fopen(token2, "r");
			if (fp_cities == NULL) {
				printf("Error: File %s is missing, skipped\n", token2);
			}
			else {
				fseek(fp_cities, 0, SEEK_END);
				if (ftell(fp_cities) == 0) {
					printf("File %s (country %s) is empty\n", token2, token1);
				}
				else {
					rewind(fp_cities);
					char buffer_cities[MAX_LENGHT];

					while (fgets(buffer_cities, MAX_LENGHT, fp_cities)!=NULL) {
						buffer_cities[strcspn(buffer_cities, "\n")] = 0;
						char* city = strtok(buffer_cities, ",");
						char* citizens = strtok(NULL, " ");

						if (city && citizens) {
							int populus = atoi(citizens);
							Root_City = InsertInTree(Root_City, city, populus);
						}
					}
				}
				fclose(fp_cities);
			}
			SortCountry(Head, token1, Root_City);
		}
	}
	fclose(fp_countries);
	return EXIT_SUCCESS;
}

int UserInquery(CountryPosition Head) {
	char target[50];
	int populus = 0;
	int found = 0;

	printf("\n Enter the name of the country: ");
	scanf("%s", target);

	while (Head != NULL) {
		if (strcmp(Head->country_name, target) == 0) {
			found = 1;
			printf("\nEnter the min. population: ");
			scanf("%d", &populus);

			printf("\n The cities in %s with a population bigger than %d are: ", target, populus);
			if (Head->Root == NULL) {
				printf("The list for this country is empty");
			}
			else {
				FindCities(Head->Root, populus);
			}
			break;
		}
		Head = Head->Next;
	}

	if (!found) {
		printf("\n The country %s is not in the list\n", target);
	}
	return EXIT_SUCCESS;
}

int FindCities(CityPosition Root, int populus) {
	if (Root == NULL) { return EXIT_SUCCESS; }

	FindCities(Root->Left, populus);
	if (Root->population > populus) {
		printf("\n City: %s with population %d", Root->city_name, Root->population);
	}
	FindCities(Root->Right, populus);

	return EXIT_SUCCESS;
}

CountryPosition AllocateMemoryCountry(char* name, CityPosition Root_City) {
	CountryPosition NewEl = (CountryPosition)malloc(sizeof(Country));
	if (NewEl == NULL) {
		printf("Error: It was not possible to allocate the memory\n");
		return NULL;
	}
	strcpy(NewEl->country_name, name);
	NewEl->Root = Root_City;
	NewEl->Next = NULL;

	return NewEl;
}

CityPosition AllocateMemoryCity(char* name, int populus){
	CityPosition NewEl = (CityPosition)malloc(sizeof(City));
	if (NewEl == NULL) {
		printf("Error: It was not possible to allocate the memory\n");
		return NULL;
	}
	
	strcpy(NewEl->city_name, name);
	NewEl->population = populus;
	NewEl->Left = NULL;
	NewEl->Right = NULL;

	return NewEl;
}

int SortCountry(CountryPosition Head, char* country_name, CityPosition Root_City) {
	CountryPosition current = Head;

	while (current->Next != NULL && strcmp(current->Next->country_name, country_name) < 0) {
		current = current->Next;
	}

	CountryPosition NewCountry = AllocateMemoryCountry(country_name, Root_City);

	if (NewCountry != NULL) {
		NewCountry->Next = current->Next;
		current->Next = NewCountry;
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

CityPosition InsertInTree(CityPosition Root, char* city_name, int populus) {

	if (Root == NULL) {
		return AllocateMemoryCity(city_name, populus);
	}

	if (populus < Root->population) {
		Root->Left = InsertInTree(Root->Left, city_name, populus);
	}

	else if (populus > Root->population) {
		Root->Right = InsertInTree(Root->Right, city_name, populus);
	}

	else {
		if (strcmp(city_name, Root->city_name) < 0)
			Root->Left = InsertInTree(Root->Left, city_name, populus);
		else
			Root->Right = InsertInTree(Root->Right, city_name, populus);
	}
	return Root;
}

int PrintCountries(CountryPosition Head) {
	while (Head) {
		printf("\n Country: %s\n", Head->country_name);
		if (Head->Root == NULL) {
			printf("There are no cities in the list");
		}
		else {
			PrintCities(Head->Root);
		}
		puts(" ");
		Head = Head->Next;
	}
	return EXIT_SUCCESS;
}

int PrintCities(CityPosition Root) {
	
	if (Root == NULL) { return EXIT_SUCCESS; }
		PrintCities(Root->Left);
		printf("\nCity name: %s and population: %d", Root->city_name, Root->population);
		PrintCities(Root->Right);
	
	return EXIT_SUCCESS;
}

int DeleteList(CountryPosition Head) {
	CountryPosition temp = NULL;
	while (Head->Next != NULL) {
		temp = Head->Next;
		
		temp->Root=DeleteTree(temp->Root);
		Head->Next = temp->Next;

		free(temp);
	}
	return EXIT_SUCCESS;
}

CityPosition DeleteTree(CityPosition Root) {

	if (Root == NULL) return NULL;

	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	free(Root);
	return EXIT_SUCCESS;
}