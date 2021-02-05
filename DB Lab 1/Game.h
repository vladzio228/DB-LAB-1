#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>

class Developer
{
public:
	int devIndex;
	char devName[30];
	int year;
	int firstDevAddress;

	Developer() {
		this->devIndex = -1;
		this->year = -1;
		this->firstDevAddress = -1;
	}

	Developer(char devName[30], int year, int devIndex) {
		strncpy_s(this->devName, devName, 30);
		this->year = year;
		this->devIndex = devIndex;
		this->firstDevAddress = -1;
	}
};

class Game
{
public:
	int devIndex;
	char gameName[30];
	int nextDevAddress;

	Game(char gameName[30], int devIndex) {
		strncpy_s(this->gameName, gameName, 30);
		this->nextDevAddress = -1;
		this->devIndex = devIndex;
	}
};