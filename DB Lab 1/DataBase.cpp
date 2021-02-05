#include "DataBase.h"
#include <cstdio>
#include <iostream>
using namespace std;

void DataBase::Init()
{
	FILE* file_m = fopen(filename_m, "wb+");
	fclose(file_m);
	FILE* file_s = fopen(filename_s, "wb+");
	fclose(file_s);
	FILE* file_i = fopen(filename_i, "wb+");
	fclose(file_i);
}

void DataBase::loadIndexTable()
{
	FILE* file_i = fopen(filename_i, "rb+");
	for (int i = 0; i < devCount; i++) {
		fread(&indexTable[i].index, sizeof(int), 1, file_i);
		fread(&indexTable[i].address, sizeof(int), 1, file_i);
	}
	fclose(file_i);
}

void DataBase::rewriteIndexTable()
{
	FILE* file_i = fopen(filename_i, "wb+");
	for (int i = 0; i < devCount; i++) {
		fwrite(&indexTable[i].index, sizeof(int), 1, file_i);
		fwrite(&indexTable[i].address, sizeof(int), 1, file_i);
	}
	fclose(file_i);
}

void DataBase::sortTable()
{
	IndexAddress temp;
	for (int i = 0; i < devCount; i++) {
		for (int j = 0; j < devCount; j++) {
			if ((indexTable[j].index > indexTable[j + 1].index && indexTable[j + 1].index != NULL
				&& indexTable[j].index != NULL) ||
				(indexTable[j].index == NULL && indexTable[j + 1].index != NULL))
			{
				temp = indexTable[j];
				indexTable[j] = indexTable[j + 1];
				indexTable[j + 1] = temp;
			}
		}
	}
}

int DataBase::getAddress(int index)
{
	for (int i = 0; i < devCount; i++) {
		if (indexTable[i].index == index) {
			return indexTable[i].address;
		}
	}
	return -1;
}

void DataBase::deleteOnIndex(int index)
{
	for (int i = 0; i < devCount; i++) {
		if (indexTable[i].index == index) {
			indexTable[i].index = NULL;
			indexTable[i].address = NULL;
			return;
		}
	}
}

void DataBase::rewriteAddressOfGame(int oldAddress, int newAddress)
{
	FILE* file_m = fopen(filename_m, "rb+");
	int DevAddress = -1;
	for (int i = 0; i < devCount; i++) {
		fseek(file_m, 38, SEEK_CUR);
		fread(&DevAddress, sizeof(int), 1, file_m);
		if (DevAddress == oldAddress) {
			fseek(file_m, -4, SEEK_CUR);
			fwrite(&newAddress, sizeof(int), 1, file_m);
			break;
		}
	}
	fclose(file_m);
	DevAddress = -1;
	FILE* file_s = fopen(filename_s, "rb+");
	for (int i = 0; i < gameCount; i++) {
		fseek(file_s, 30, SEEK_CUR);
		fread(&DevAddress, sizeof(int), 1, file_s);
		if (DevAddress == oldAddress) {
			fseek(file_s, -4, SEEK_CUR);
			fwrite(&newAddress, sizeof(int), 1, file_s);
			break;
		}
	}
	fclose(file_s);
}

void DataBase::ut_s()
{
	cout << "ut_s:" << endl;
	FILE* file = fopen(filename_s, "rb+");
	char gameName[30];
	int nextGameAddress;
	for (int i = 0; i < gameCount; i++) {
		fread(&gameName, sizeof(gameName), 1, file);
		fread(&nextGameAddress, sizeof(int), 1, file);
		cout << "Game name: " << gameName << ", next game address: "
			<< nextGameAddress << endl;
	}
}

void DataBase::ut_m()
{
	cout << "ut_m:" << endl;
	FILE* file = fopen(filename_m, "rb+");
	int index, year, firstDevAddress;
	char devName[30];
	for (int i = 0; i < devCount; i++) {
		fread(&index, sizeof(int), 1, file);
		fread(&devName, sizeof(devName), 1, file);
		fread(&year, sizeof(int), 1, file);
		fread(&firstDevAddress, sizeof(int), 1, file);
		cout << "index: " << index << ", dev name: " << devName <<
			", year: " << year << ", first game address: " << firstDevAddress << endl;
	}

}


void DataBase::get_m(int index)
{
	int offset = getAddress(index);
	if (offset == -1) {
		cout << "There is no developer at index " << index << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	fseek(file_m, offset, SEEK_SET);
	int devIndex, year, firstDevAddress;
	char devName[30];
	fread(&devIndex, sizeof(int), 1, file_m);
	fread(&devName, sizeof(devName), 1, file_m);
	fread(&year, sizeof(int), 1, file_m);
	fread(&firstDevAddress, sizeof(int), 1, file_m);
	cout << "developer at index " << devIndex << " = dev name: " << devName <<
		", year: " << year << endl;
	fclose(file_m);
}

void DataBase::get_s(int index_s, int index_m)
{
	int offset_m = getAddress(index_m);
	if (offset_m == -1) {
		cout << "There is no developer at index " << index_m << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	fseek(file_m, offset_m + 38, SEEK_SET);
	int firstDevAddress;
	fread(&firstDevAddress, sizeof(int), 1, file_m);
	fclose(file_m);
	if (firstDevAddress == -1) {
		cout << "There are no games of this developer" << endl;
		return;
	}
	else {
		FILE* file_s = fopen(filename_s, "rb+");
		fseek(file_s, firstDevAddress, SEEK_SET);
		char gameName[30];
		if (index_s == 1)
		{
			fread(&gameName, sizeof(gameName), 1, file_s);
			cout << "developer index " << index_m << ". Game name of with index " <<
				index_s << ": " << gameName << endl;
		}
		else {
			fseek(file_s, 30, SEEK_CUR);
			int nextDevAddress;
			for (int i = 1; i < index_s; i++) {
				fread(&nextDevAddress, sizeof(int), 1, file_s);
				fseek(file_s, nextDevAddress + 30, SEEK_SET);
			}
			if (nextDevAddress < 0) {
				cout << "There is no game with index " << index_s <<
					" for a developer with index " << index_m << endl;
				return;
			}
			fseek(file_s, -30, SEEK_CUR);
			fread(&gameName, sizeof(gameName), 1, file_s);
			cout << "developer index " << index_m << ". Game name of game with index " <<
				index_s << ": " << gameName << endl;
		}
		fclose(file_s);
	}
}

void DataBase::delete_m(int index)
{
	int offset_m = getAddress(index);
	if (offset_m == -1) {
		cout << "There is no developer at index " << index << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	int offsetFromEnd = sizeof(Developer) - 2;
	fseek(file_m, devEndAddress - offsetFromEnd, SEEK_SET);
	int devIndex, year, firstDevAddress;
	char devName[30];
	int deletedFirstDevAddress;
	fread(&devIndex, sizeof(int), 1, file_m);
	fread(&devName, sizeof(devName), 1, file_m);
	fread(&year, sizeof(int), 1, file_m);
	fread(&firstDevAddress, sizeof(int), 1, file_m);

	fseek(file_m, offset_m + 38, SEEK_SET);
	fread(&deletedFirstDevAddress, sizeof(int), 1, file_m);
	fseek(file_m, offset_m, SEEK_SET);

	fwrite(&devIndex, sizeof(int), 1, file_m);
	fwrite(&devName, sizeof(devName), 1, file_m);
	fwrite(&year, sizeof(int), 1, file_m);
	fwrite(&firstDevAddress, sizeof(int), 1, file_m);

	fclose(file_m);
	if (deletedFirstDevAddress != -1) {
		int nextDevAddress = deletedFirstDevAddress;
		FILE* file_s = fopen(filename_s, "rb+");
		char rewritedGameName[30];
		int rewritedNextDevAddress;
		int deletedAdress;
		while (nextDevAddress != -1) {
			deletedAdress = nextDevAddress;
			fseek(file_s, nextDevAddress + 30, SEEK_SET);
			fread(&nextDevAddress, sizeof(int), 1, file_s);
			fseek(file_s, gameEndAddress - 34, SEEK_SET);
			fread(&rewritedGameName, sizeof(rewritedGameName), 1, file_s);
			fread(&rewritedNextDevAddress, sizeof(int), 1, file_s);
			fseek(file_s, deletedAdress, SEEK_SET);
			fwrite(&rewritedGameName, sizeof(rewritedGameName), 1, file_s);
			fwrite(&rewritedNextDevAddress, sizeof(int), 1, file_s);
			rewriteAddressOfGame(gameEndAddress - 34, deletedAdress);
			gameEndAddress -= sizeof(Game) - 6;
			gameCount--;
		}
		fclose(file_s);
	}

	devEndAddress -= sizeof(Developer) - 2;
	loadIndexTable();
	deleteOnIndex(index);
	sortTable();
	rewriteIndexTable();
	devCount--;
}

void DataBase::delete_s(int index_s, int index_m)
{
	int offset_m = getAddress(index_m);
	if (offset_m == -1) {
		cout << "There is no developer at index " << index_m << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	int firstDevAddress;
	fseek(file_m, offset_m + 38, SEEK_SET);
	fread(&firstDevAddress, sizeof(int), 1, file_m);
	if (firstDevAddress == -1) {
		cout << "There are no games of this developer" << endl;
		return;
	}
	else {
		FILE* file_s = fopen(filename_s, "rb+");
		int nextDevAddress = firstDevAddress;
		for (int i = 1; i < index_s; i++) {
			fseek(file_s, nextDevAddress + 30, SEEK_SET);
			fread(&nextDevAddress, sizeof(int), 1, file_s);
		}
		int deletedAddress = nextDevAddress;
		if (nextDevAddress == -1) {
			cout << "There is no game with index " << index_s <<
				" for a developer with index " << index_m;
			return;
		}
		fseek(file_s, gameEndAddress - 34, SEEK_SET);
		char rewritedGameName[30];
		int rewritedNextDevAddress;
		int deletedNextAddress;
		fread(&rewritedGameName, sizeof(rewritedGameName), 1, file_s);
		fread(&rewritedNextDevAddress, sizeof(int), 1, file_s);
		fseek(file_s, deletedAddress + 30, SEEK_SET);
		fread(&deletedNextAddress, sizeof(int), 1, file_s);
		fseek(file_s, -34, SEEK_CUR);
		fwrite(&rewritedGameName, sizeof(rewritedGameName), 1, file_s);
		fwrite(&rewritedNextDevAddress, sizeof(int), 1, file_s);
		fclose(file_s);
		rewriteAddressOfGame(deletedAddress, deletedNextAddress);
		rewriteAddressOfGame(gameEndAddress - 34, deletedAddress);
		gameEndAddress -= sizeof(Game) - 6;
		gameCount--;
	}
	fclose(file_m);
}

void DataBase::update_m(int index, char newDevName[30], int newYear)
{
	int offset = getAddress(index);
	if (offset == -1) {
		cout << "There is no developer at index " << index << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	fseek(file_m, offset + 4, SEEK_SET);
	fwrite(newDevName, 30, 1, file_m);
	fwrite(&newYear, sizeof(int), 1, file_m);
	fclose(file_m);
}

void DataBase::update_s(int index_s, int index_m, char newGameName[30])
{
	int offset_m = getAddress(index_m);
	if (offset_m == -1) {
		cout << "There is no developer at index " << index_m << endl;
		return;
	}
	FILE* file_m = fopen(filename_m, "rb+");
	fseek(file_m, offset_m + 38, SEEK_SET);
	int firstDevAddress;
	fread(&firstDevAddress, sizeof(int), 1, file_m);
	fclose(file_m);
	if (firstDevAddress == -1) {
		cout << "There are no games of this developer" << endl;
		return;
	}
	else {
		FILE* file_s = fopen(filename_s, "rb+");
		fseek(file_s, firstDevAddress, SEEK_SET);
		if (index_s == 1)
		{
			fwrite(newGameName, 30, 1, file_s);
		}
		else {
			fseek(file_s, 30, SEEK_CUR);
			int nextDevAddress;
			for (int i = 1; i < index_s; i++) {
				fread(&nextDevAddress, sizeof(int), 1, file_s);
				fseek(file_s, nextDevAddress + 30, SEEK_SET);
			}
			if (nextDevAddress < 0) {
				cout << "There is no game with index " << index_s <<
					" for a developer with index " << index_m << endl;
				return;
			}
			fseek(file_s, -30, SEEK_CUR);
			fwrite(newGameName, 30, 1, file_s);
		}
		fclose(file_s);
	}
}

void DataBase::insert_m(Developer* developer)
{
	for (int i = 0; i < devCount; i++) {
		if (indexTable[i].index == developer->devIndex) {
			cout << "This key is allready used" << endl;
			return;
		}
	}
	FILE* file = fopen(filename_m, "rb+");
	loadIndexTable();
	indexTable[devCount].index = developer->devIndex;
	indexTable[devCount].address = devCount * (sizeof(Developer) - 2);
	fseek(file, devEndAddress, SEEK_SET);
	devEndAddress += sizeof(Developer) - 2;

	fwrite(&developer->devIndex, sizeof(int), 1, file);
	fwrite(&developer->devName, sizeof(developer->devName), 1, file);
	fwrite(&developer->year, sizeof(int), 1, file);
	fwrite(&developer->firstDevAddress, sizeof(int), 1, file);

	sortTable();
	rewriteIndexTable();
	devCount++;
	fclose(file);
}

void DataBase::insert_s(Game* game)
{
	FILE* file_s = fopen(filename_s, "rb+");
	fseek(file_s, gameEndAddress, SEEK_SET);
	gameEndAddress += (sizeof(Game) - 6);
	fwrite(&game->gameName, sizeof(game->gameName), 1, file_s);
	fwrite(&game->nextDevAddress, sizeof(int), 1, file_s);

	//38 is the size of fields before firstDevAddress
	int offset = getAddress(game->devIndex) + 38;
	FILE* file_m = fopen(filename_m, "rb+");
	fseek(file_m, offset, SEEK_SET);
	int firstDevAddress;
	fread(&firstDevAddress, sizeof(int), 1, file_m);
	int address = gameCount * (sizeof(Game) - 6);
	if (firstDevAddress == -1) {
		fseek(file_m, -4, SEEK_CUR);
		fwrite(&address, sizeof(int), 1, file_m);
	}
	else {
		int nextDevAddress = firstDevAddress;
		while (nextDevAddress != -1) {
			fseek(file_s, nextDevAddress + 30, SEEK_SET);
			fread(&nextDevAddress, sizeof(int), 1, file_s);
		}
		fseek(file_s, -4, SEEK_CUR);
		fwrite(&address, sizeof(int), 1, file_s);
	}

	gameCount++;
	fclose(file_s);
	fclose(file_m);
}