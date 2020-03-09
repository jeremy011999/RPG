#include "game.h"
#include "entity.h"
#include <Windows.h>

using namespace std;

/*
15=white
12=red
*/

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
	SetConsoleTextAttribute(console, 15);
	cout << "-----------------------Welcome!---------------------" << endl << endl << endl;
	cout << "    This is a text based adventure game called:" << endl << endl;
	SetConsoleTextAttribute(console, 10);
	cout << "  _______  ______  ______  ______  ___ ___" << endl;
	cout << "     |     |       |    |  |      |   |   | \\     /" << endl;
	cout << "     |     |       |   /   |      |   |   |  \\   /" << endl;
	cout << "     |     |___    | /     |___   |   |   |   \\ /" << endl;
	cout << "     |     |       | \\     |      |   |   |    |" << endl;
	cout << "     |     |       |  \\    |      |   |   |    |" << endl;
	cout << "_____|     |_____  |   \\   |_____ |   |   |    |" << endl << endl;
	cout << "                         is" << endl;
	cout << "                       bored!" << endl << endl << endl;
	SetConsoleTextAttribute(console, 15);
	system("pause");
	system("CLS");
	cout << "Please choose difficulty\n1.Easy\n2.Normal\n3.Hard\n";
	int difficulty, size;
		cin >> difficulty;
		switch (difficulty)
		{
		case 1:
		{
			cout << "You are playing on Easy difficulty. Mobs will spawn at a lower rate.\n";
			break;
		}
		case 2:
		{
			cout << "You are playing on Normal difficulty. Mobs will spawn at a normal rate.\n";
			break;
		}
		case 3:
		{
			cout << "You are playing on Hard difficulty. Mobs will spawn at a higher rate.\n";
			break;
		}
		default:
		{
			"Please make a valid choice.";
		}
		}
	system("Pause");
	system("CLS");
	while (1)
	{
		cout << "Please select a board size NxN (10-25)\nN= ";
		cin >> size;
		if (size < 10 || size>25)
		{
			cout << "Please select a board size NxN (10-25)\nN= ";
			cin >> size;
		}
		else
			break;
	}
	system("CLS");
	game bored(size, difficulty);
	bored.start();
	return 0;
}