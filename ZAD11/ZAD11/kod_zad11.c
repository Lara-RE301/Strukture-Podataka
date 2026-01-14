#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Structures.h"
#define MAX_LENGHT 100

HashTablePosition CreatTheTable(int value);
int FindPrimeNumber(int x);
int IsItPrime(int x);
int CalculateKey(char* country_name, int table_size);

int SortCountryInHash(CountryPosition Head, char* country_name, CityPosition Root_City);
int UserInquery(HashTablePosition hash_table);
int ReadFromFile(HashTablePosition hash_table);
CountryPosition AllocateMemoryCountry(char* name, CityPosition Root_City);
CityPosition AllocateMemoryCity(char* name, int populus);
CityPosition InsertInTree(CityPosition Root, char* city_name, int populus);

int PrintHashTable(HashTablePosition hash_table);
int PrintCities(CityPosition Root);
int FindCities(CityPosition Root, int populus);
int DeleteTable(HashTablePosition table);
CityPosition DeleteTree(CityPosition Root);


int main() {
	Country Head = { .country_name = "", .Next = NULL, .Root = NULL };
	int table_size;

	printf("What is the size of the hash table?");
	scanf("%d", &table_size);

	HashTablePosition table = CreatTheTable(table_size);
	if (table == NULL) return EXIT_FAILURE;

	if (ReadFromFile(table) == EXIT_SUCCESS) {
		printf("\n----All countries and cities in the files----\n");
		PrintHashTable(table);
		printf("----User Inquery----\n");
		UserInquery(table);
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

	Hash_table->HashList = (CountryPosition*)malloc(sizeof(Country) * Hash_table->size);
	if (Hash_table->HashList == NULL) {
		printf("Error: It was not possible to allocate the memory\n");
		free(Hash_table);
		return NULL;
	}
	for (i = 0; i < Hash_table->size; i++) {
		Hash_table->HashList[i] = AllocateMemoryCountry("", NULL);
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

int ReadFromFile(HashTablePosition hash_table) {
	char buffer[MAX_LENGHT];
	char* token1 = NULL;
	char* token2 = NULL;

	FILE* fp_countries = fopen("drzave.txt", "r");
	if (fp_countries == NULL) {
		printf("Error: It was not possible to open the file");
		return EXIT_FAILURE;
	}

	while (fgets(buffer, MAX_LENGHT, fp_countries) != NULL) {
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

					while (fgets(buffer_cities, MAX_LENGHT, fp_cities) != NULL) {
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
			int key = CalculateKey(token1, hash_table->size);
			SortCountryInHash(hash_table->HashList[key], token1, Root_City);
		}
	}
	fclose(fp_countries);
	return EXIT_SUCCESS;
}

int UserInquery(HashTablePosition hash_table) {
	char target[50];
	int populus = 0;

	printf("\n Enter the name of the country: ");
	scanf("%s", target);

	int key = CalculateKey(target, hash_table->size);
	CountryPosition current = hash_table->HashList[key]->Next;

	while (current != NULL) {
		if (strcmp(current->country_name, target) == 0) {
			printf("\nEnter the min. population: ");
			scanf("%d", &populus);
			FindCities(current->Root, populus);
			return EXIT_SUCCESS;
		}
		current = current->Next;
	}

	printf("\n The country %s is not in the list\n", target);
	return EXIT_FAILURE;
}

int SortCountryInHash(CountryPosition Head, char* country_name, CityPosition Root_City) {	
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

CityPosition AllocateMemoryCity(char* name, int populus) {
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

int PrintHashTable(HashTablePosition hash_table) {
	int i;
	for (i = 0; i < hash_table->size; i++) {
		CountryPosition current = hash_table->HashList[i]->Next;
		if (current) printf("\n [%d]: ", i);
		while (current) {
			printf("\n Country: %s", current->country_name);
			PrintCities(current->Root);
		}
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

int FindCities(CityPosition Root, int populus) {
	if (Root == NULL) { return EXIT_SUCCESS; }

	FindCities(Root->Left, populus);
	if (Root->population > populus) {
		printf("\n City: %s with population %d", Root->city_name, Root->population);
	}
	FindCities(Root->Right, populus);

	return EXIT_SUCCESS;
}

int DeleteTable(HashTablePosition table) {
	int i;
	if (table==NULL) return;
	for (i = 0; i < table->size; i++) {
		CountryPosition current = table->HashList[i];
		while (current != NULL) {
			CountryPosition temp = current;
			current = current->Next;
			temp->Root = DeleteTree(temp->Root);
			free(temp);
		}
	}
	free(table->HashList);
	free(table);
	return EXIT_SUCCESS;
}

CityPosition DeleteTree(CityPosition Root) {

	if (Root == NULL) return NULL;

	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	free(Root);
	return EXIT_SUCCESS;
}