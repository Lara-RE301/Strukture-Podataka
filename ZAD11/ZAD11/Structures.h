#ifndef STRUCTURES_H
#define SUTRCTURES_H

typedef struct Country* CountryPosition;
typedef struct City* CityPosition;
typedef struct HashTable* HashTablePosition;

typedef struct Country {
	char country_name[50];
	int key;
	CountryPosition Next;
	CityPosition Root;
}Country;
typedef struct City {
	char city_name[50];
	int population;
	CityPosition Left;
	CityPosition Right;
}City;
typedef struct HashTable {
	int size;
	CountryPosition* HashList;
}HashTable;


#endif
