#include<iostream>
#include"game.h"
#include <windows.h>

using namespace std;

void main() 
{
	Game game;
	game.init();
	game.home();
	game.print();
	system("cls");
	while (true)
	{
		game.update();
		game.print();
		//Sleep(500);
	}
}