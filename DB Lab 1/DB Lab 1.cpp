#include <iostream>
#include "DataBase.h"
using namespace std;

int main()
{
	DataBase dataBase;
	dataBase.Init();
	//1: Inserting master-records
	char developer1Name[30] = "Bethesda";
	char developer2Name[30] = "GSC";
	char developer3Name[30] = "Ubisoft";
	char developer4Name[30] = "CD Project Red";
	char developer5Name[30] = "id Software";
	Developer developer1(developer1Name, 1986, 1);
	Developer developer2(developer2Name, 1995, 29);
	Developer developer3(developer3Name, 1986, 10);
	Developer developer4(developer4Name, 2002, 2);
	Developer developer5(developer5Name, 1991, 3);
	dataBase.insert_m(&developer1);
	dataBase.insert_m(&developer2);
	dataBase.insert_m(&developer3);
	dataBase.insert_m(&developer4);
	dataBase.insert_m(&developer5);
	//2 Insert slave-records
	char game1Name[30] = "Fallout 4";
	char game2Name[30] = "Stalker: Clear sky";
	char game3Name[30] = "Assassins creed 2";
	char game4Name[30] = "The Witcher 3: Wild Hunt";
	char game5Name[30] = "Doom Eternal";
	char game6Name[30] = "Cyberpunk 2077";
	Game game1(game1Name, 1);
	Game game2(game2Name, 29);
	Game game3(game3Name, 10);
	Game game4(game4Name, 2);
	Game game5(game5Name, 3);
	Game game6(game6Name, 2);
	dataBase.insert_s(&game1);
	dataBase.insert_s(&game2);
	dataBase.insert_s(&game3);
	dataBase.insert_s(&game4);
	dataBase.insert_s(&game5);
	dataBase.insert_s(&game6);
	//3	Output all master and slave records
	cout << "Step 3:" << endl;
	dataBase.ut_m();
	dataBase.ut_s();
	cout << endl;
	//4 Deleting master record with id 29
	dataBase.delete_m(29);
	//5 Deleting slave record number 1 with master id 2
	dataBase.delete_s(1, 2);
	//6 Output all master and slave records
	cout << "Step 6:" << endl;
	dataBase.ut_m();
	dataBase.ut_s();
	cout << endl;
	//7 Inserting new master and slave records
	char developer6Name[30] = "Valve";
	Developer developer6(developer6Name, 1996, 4);
	char game7Name[30] = "Dota 2";
	Game game7(game7Name, 4);
	dataBase.insert_m(&developer6);
	dataBase.insert_s(&game7);
	//8 Output all master and slave records
	cout << "Step 8:" << endl;
	dataBase.ut_m();
	dataBase.ut_s();
	cout << endl;
	//9 Updating new master record and slave record
	char developer7Name[30] = "From Software";
	dataBase.update_m(4, developer7Name, 1986);
	char game8Name[30] = "Dark souls 3";
	dataBase.update_s(1, 4, game8Name);
	//10 Output all master and slave record
	cout << "Step 10:" << endl;
	dataBase.ut_m();
	dataBase.ut_s();
	cout << endl;

}
