#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"

#define MAX_SIZE 100

HashTablePosition CreatTheTable(int value);
int FindPrimeNumber(int x);
int IsItPrime(int x);
int CalculateKey(char* country_name, int table_size);

int ReadFile(HashTablePosition hash_table);
PositionCountry InsertCountryInTree(PositionCountry Root, PositionCountry Country);
PositionCity InsertCityInList(PositionCity Head, char* city_name, int populus);
PositionCountry AllocateMemoryCountry(char* name);
PositionCity AllocateMemoryCity(char* name, int populus);

int PrintHashTable(HashTablePosition hash_table);
int PrintCountriesTree(PositionCountry Root);
int PrintCityList(PositionCity Head);
int Inquery(HashTablePosition table);
int FindCitiesInList(PositionCity Head, int populus);

int DeleteTable(HashTablePosition table);
PositionCountry DeleteTree(PositionCountry Root);
int DeleteList(PositionCity Head);

int main() {
	int table_size;

	printf("What is the size of the hash table?");
	scanf("%d", &table_size);

	HashTablePosition table = CreatTheTable(table_size);
	if (table == NULL) return EXIT_FAILURE;

	if (ReadFile(table) == EXIT_SUCCESS) {
		printf("\n----All countries and cities in the files----\n");
		PrintCountriesTree(table);
		printf("----User Inquery----\n");
		Inquery(table);
	}

	DeleteTable(table);
	return 0;
}

HashTablePosition CreatTheTable(int value) {
	HashTablePosition Hash_table;
	int i;
	Hash_table = (HashTablePosition)malloc(sizeof(HashTable));
	if (Hash_table == NULL) {
		printf("Error: It was not possible to allocate the memory\n");
		return NULL;
	}
	Hash_table->size = FindPrimeNumber(value);

	Hash_table->HashList = (PositionCity*)malloc(sizeof(CityList) * Hash_table->size);
	if (Hash_table->HashList == NULL) {
		printf("Error: It was not possible to allocate the memory\n");
		free(Hash_table);
		return NULL;
	}
	for (i = 0; i < Hash_table->size; i++) {
		Hash_table->HashList[i] = NULL;
	}
	return Hash_table;
}

int FindPrimeNumber(int x) {
	while (!IsItPrime(x)) {
		x++;
	}
	return x;
}
int IsItPrime(int x) {
	if (x <= 1) return 0;
	if (x <= 3) return 1;
	if (x % 2 == 0 || x % 3 == 0) return 0;
	for (int i = 5; i * i <= x; i = i + 6)
		if (x % i == 0 || x % (i + 2) == 0)
			return 0;
	return 1;
}

int CalculateKey(char* country_name, int table_size) {
	int i = 0, suma = 0;
	for (i = 0; i < 5 && country_name != "\0"; i++) {
		suma += (int)country_name[i];
	}
	return suma % table_size;
}


int ReadFile(HashTablePosition hash_table) {
	char buffer[MAX_SIZE];

	FILE* fp_countries = fopen("drzave.txt", "r");
	if (fp_countries == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
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
				int key = CalculateKey(token1, hash_table->size);
				hash_table->HashList = InsertCountryInTree(hash_table->HashList[key], NewCountry);
			}
		}
	}
	fclose(fp_countries);
	return EXIT_SUCCESS;
}

int Inquery(HashTablePosition table) {
	char target[50];
	int populus;

	printf("\n Enter the name of the country: ");
	scanf("%s", target);


	int key = CalculateKey(target, table->size);
	PositionCountry current = table->HashList[key];

	while (current != NULL) {
		if (strcmp(current->country, target) == 0) {
			printf("\nEnter the min. population: ");
			scanf("%d", &populus);

			printf("\n The cities in %s with a population bigger than %d are: ", target, populus);
			FindCitiesInList(current->City_Root, populus);
			return EXIT_SUCCESS;
		}
		else if (strcmp(target, current->country) < 0)
			current = current->Left;

		else current = current->Right;
	}

	printf("\n The country %s is not in the list\n", target);

	return EXIT_FAILURE;
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

int PrintHashTable(HashTablePosition hash_table) {
	int i;
	for (i = 0; i < hash_table->size; i++) {
		if (hash_table->HashList != NULL) {
			printf("[%d] ", i);
			PrintCountriesTree(hash_table->HashList[i]);
		}
	}
	return EXIT_SUCCESS;
}

int PrintCountriesTree(PositionCountry Root) {
	if (Root == NULL) { return EXIT_SUCCESS; }

	PrintCountriesTree(Root->Left);
	printf("\nDrzava: %s", Root->country);
	PrintCityList(Root->City_Root);
	PrintCountriesTree(Root->Right);

	return EXIT_SUCCESS;
}

int PrintCityList(PositionCity Head) {

	while (Head != NULL) {
		printf("City: %s with population of %d citizens\n", Head->city, Head->inhabitant);
		Head = Head->Next;
	}
	return EXIT_SUCCESS;
}

int DeleteTable(HashTablePosition table) {
	for (int i = 0; i < table->size; i++) {
		table->HashList[i] = DeleteTree(table->HashList[i]);
	}
	free(table->HashList);
	free(table);

	return EXIT_SUCCESS;
}

PositionCountry DeleteTree(PositionCountry Root) {
	if (Root == NULL) return NULL;

	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	DeleteListCity(Root->City_Root);
	free(Root);

	return NULL;
}

int DeleteList(PositionCity Head) {
	while (Head != NULL) {
		PositionCity temp = Head;
		Head = Head->Next;
		free(temp);
	}
	return NULL;
}
