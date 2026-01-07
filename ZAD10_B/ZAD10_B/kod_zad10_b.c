#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct CityList* PositionCity;
typedef struct CountryTree* PositionCountry;

typedef struct CityList {
	char city[50];
	int inhabitant;
	PositionCity Next;
}CityList;
typedef struct CountryTree {
	char country[50];
	PositionCity City_Root;
	PositionCountry Left;
	PositionCountry Right;
}CountryTree;

PositionCountry ReadFile(PositionCountry Root);
PositionCountry AllocateMemoryCountry(char* name);
PositionCity AllocateMemoryCity(char* name, int populus);
PositionCity InsertCityInList(PositionCity Head, char* city_name, int populus);
PositionCountry InsertCountryInTree(PositionCountry Root, PositionCountry Country);
PositionCountry DeleteListCity(PositionCity Head);
PositionCity DeleteAll(PositionCountry Root);
int PrintCityList(PositionCity Head);
int PrintCountriesTree(PositionCountry Root);
int Inquery(PositionCountry Root);
int FindCitiesInList(PositionCity Head, int populus);

int main() {
	PositionCountry Root = NULL;
	Root = ReadFile(Root);
	if (Root != NULL) {
		printf("\n----All countries and cities in the files----\n");
		PrintCountriesTree(Root);
		printf("----User Inquery----\n");
		Inquery(Root);
	}
	Root = DeleteAll(Root);

	return 0;
}

PositionCountry ReadFile(PositionCountry Root) {
	char buffer[MAX_SIZE];

	FILE* fp_countries = fopen("drzave.txt", "r");
	if (fp_countries == NULL) {
		printf("Error: It was not possible to open the file");
		return NULL;
	}

	while (fgets(buffer, MAX_SIZE, fp_countries) != NULL) {
		buffer[strcspn(buffer, "\n")] = 0;

		char* token1 = strtok(buffer, " ");
		char* token2 = strtok(NULL, " ");

		if (token1 && token2) {
			PositionCountry NewCountry = AllocateMemoryCountry(token1);
			if (NewCountry == NULL) continue;

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
					char buffer_cities[MAX_SIZE];

					while (fgets(buffer_cities, MAX_SIZE, fp_cities) != NULL) {
						buffer_cities[strcspn(buffer_cities, "\n")] = 0;
						char* city = strtok(buffer_cities, ",");
						char* citizens = strtok(NULL, " ");

						if (city && citizens) {
							int populus = atoi(citizens);
							NewCountry->City_Root = InsertCityInList(NewCountry->City_Root, city, populus);
						}
					}
					fclose(fp_cities);
				}
				Root = InsertCountryInTree(Root, NewCountry);
			}
		}
	}
	fclose(fp_countries);
	return Root;
}

int Inquery(PositionCountry Root) {
	char target[50];
	int populus;

	printf("\n Enter the name of the country: ");
	scanf("%s", target);

	PositionCountry current = Root;
	while (current != NULL) {
		if (strcmp(Root->country, target) == 0) {
			printf("\nEnter the min. population: ");
			scanf("%d", &populus);

			printf("\n The cities in %s with a population bigger than %d are: ", target, populus);
			FindCitiesInList(Root->City_Root, populus);
			return EXIT_SUCCESS;
		}
		else if (strcmp(target, Root->country) < 0) 
			current = current->Left;

		else current = current->Right;
	}

	printf("\n The country %s is not in the list\n", target);
	
	return EXIT_SUCCESS;
}

PositionCountry AllocateMemoryCountry(char* name) {
	PositionCountry NewEl = (PositionCountry)malloc(sizeof(CountryTree));
	if (NewEl == NULL) {
		printf("Error: It was not possible to allocate the memory");
		return NULL;
	}
	strcpy(NewEl->country, name);
	NewEl->City_Root = NULL;
	NewEl->Left = NULL;
	NewEl->Right = NULL;

	return NewEl;
}

PositionCity AllocateMemoryCity(char* name, int populus) {
	PositionCity NewEl = (PositionCity)malloc(sizeof(CityList));
	if (NewEl == NULL) {
		printf("Error: It was not possible to allocate the memory");
		return NULL;
	}
	strcpy(NewEl->city, name);
	NewEl->inhabitant = populus;

	return NewEl;
}

PositionCity InsertCityInList(PositionCity Head, char* city_name, int populus) {
	PositionCity NewCity = AllocateMemoryCity(city_name, populus);
	if (NewCity == NULL) { return Head; }

	if (Head == NULL || populus < Head->inhabitant || (populus == Head->inhabitant && strcmp(city_name, Head->city) < 0)) {
		NewCity->Next = Head;
		return NewCity;
	}

	PositionCity current = Head;
	while (current->Next != NULL && (current->Next->inhabitant < populus || (current->Next->inhabitant == populus && strcmp(current->Next->city, city_name) < 0))) {
		current = current->Next;
	}

	NewCity->Next = current->Next;
	current->Next = NewCity;
	return Head;
}

PositionCountry InsertCountryInTree(PositionCountry Root, PositionCountry Country) {
	if (Root == NULL) { return Country; }

	if (strcmp(Country->country, Root->country) < 0) {
		Root->Left = InsertCountryInTree(Root->Left, Country);
	}
	else if (strcmp(Country->country, Root->country) > 0) {
		Root->Right = InsertCountryInTree(Root->Right, Country);
	}
	else {
		printf("\n This country is already in the tree");
		free(Country);
	}
	return Root;
}

int FindCitiesInList(PositionCity Head, int populus) {
	int found = 0;
	while (Head != NULL) {
		if (Head->inhabitant > populus) {
			printf("City: %s and its population %d", Head->city, Head->inhabitant);
			found = 1;
		}
		Head = Head->Next;
	}
	if (!found) printf("\n All cities have a population minor to the one entered");
	return EXIT_SUCCESS;
}

int PrintCountriesTree(PositionCountry Root) {
	if (Root != NULL) {
		PrintCountriesTree(Root->Left);
		printf("\nCountry: %s\n", Root->country);
		if (Root->City_Root == NULL) { printf("\n There is no city"); }
		else
			PrintCityList(Root->City_Root);
		puts(" ");
		PrintCountriesTree(Root->Right);
	}
}

int PrintCityList(PositionCity Head) {

	while (Head != NULL) {
		printf("City: %s with population of %d citizens\n", Head->city, Head->inhabitant);
		Head = Head->Next;
	}
	return EXIT_SUCCESS;
}

PositionCountry DeleteListCity(PositionCity Head) {
	while (Head != NULL) {
		PositionCity temp = Head;
		Head = Head->Next;
		free(temp);
	}
	return NULL;
}
PositionCity DeleteAll(PositionCountry Root) {
	if (Root == NULL) return NULL;

	DeleteAll(Root->Left);
	DeleteAll(Root->Right);
	DeleteListCity(Root->City_Root);
	free(Root);

	return NULL;
}