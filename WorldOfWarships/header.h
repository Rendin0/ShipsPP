#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <thread>
#pragma comment(lib, "winmm.lib")

class Player
{
	std::vector<std::vector<int>> field;
	std::vector<std::vector<int>> all_ships;
	std::vector<std::vector<int>> dead_ships;
public:
	Player();
	std::vector<std::vector<int>> getField();
	void setPoint(std::vector<int> point, int state);
	bool getState();
	void shipIsDead(size_t row, size_t line, int direction, size_t ship_id);
};
std::vector<std::vector<std::vector<int>>> fieldCreate();
std::vector<std::vector<std::vector<int>>> fieldCreateRandom();
void fieldPrint(std::vector<std::vector<int>> field, bool fogOfWar, std::vector<int> point);

class Game
{
	Player player1;
	Player player2;
	int turn;
public:
	Game();
	void fieldsPrint(std::vector<int> point);
	void attack();
	int getState();
	void changeTurn();
};
int localGame();

int menu(int state);
int choice(std::vector<std::string> str);
int choice(std::string text, std::vector<std::string> str);

