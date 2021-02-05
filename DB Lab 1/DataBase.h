#pragma once
#include "Game.h"
#define _CRT_SECURE_NO_DEPRECATE
#define SIZE 1000

struct IndexAddress {
	int index;
	int address;
};

class DataBase
{
public:
	void get_m(int index);
	void get_s(int index_s, int index_m);
	void delete_m(int index);
	void delete_s(int index_s, int index_m);
	void update_m(int index, char newDevName[30], int newYear);
	void update_s(int index_s, int index_m, char newGameName[30]);
	void insert_m(Developer* developer);
	void insert_s(Game* game);
	void ut_m();
	void ut_s();
	void Init();
private:
	int devCount = 0;
	int gameCount = 0;

	int devEndAddress = 0;
	int gameEndAddress = 0;
	IndexAddress indexTable[SIZE];

	const char* filename_m = "Developers.fl";
	const char* filename_s = "Games.fl";
	const char* filename_i = "DevelopersIndexes.ind";

	void loadIndexTable();
	void rewriteIndexTable();
	void sortTable();
	int getAddress(int index);
	void deleteOnIndex(int index);
	void rewriteAddressOfGame(int oldAddress, int newAddress);
};
