#ifndef STRUCTURES_H
#define SUTRCTURES_H


typedef struct CityList* PositionCity;
typedef struct CountryTree* PositionCountry;
typedef struct HashTable* HashTablePosition;

typedef struct CityList {
	char city[50];
	int inhabitant;
	int key;
	PositionCity Next;
}CityList;

typedef struct CountryTree {
	char country[50];
	PositionCity City_Root;
	PositionCountry Left;
	PositionCountry Right;
}CountryTree;

typedef struct HashTable {
	int size;
	PositionCity* HashList;
}HashTable;


#endif