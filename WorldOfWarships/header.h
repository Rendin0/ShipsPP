#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

class Player
{
	std::vector<std::vector<int>> field;
	std::vector<std::vector<int>> all_ships;
	std::vector<std::vector<int>> dead_ships;
public:
	std::vector<std::vector<int>> cpu_pointers;
	int cpu_dir;
	Player();
	Player(bool cpu);
	std::vector<std::vector<int>> getField();
	void setPoint(std::vector<int> point, int state);
	bool getState();
	void shipIsDead(size_t row, size_t line, int direction, size_t ship_id);
	void setAll(std::vector<std::vector<int>> _field, std::vector<std::vector<int>> _all_ships, std::vector<std::vector<int>> _dead_ships);
};
std::vector<std::vector<std::vector<int>>> fieldCreate();
std::vector<std::vector<std::vector<int>>> fieldCreateRandom();
std::vector<std::vector<std::vector<int>>> fieldCreateCpu();
void fieldPrint(std::vector<std::vector<int>> field, const bool fogOfWar, std::vector<int> point);

class Game
{
	Player* player1;
	Player* player2;
	int turn;
public:
	Game(bool mode);
	void fieldsPrint(std::vector<int> point1, std::vector<int> point2, const bool fogOfWar);
	void attack();
	void computerAttack();
	int getState();
	void changeTurn();
	void setPlayer(int number, std::vector<std::vector<int>> field, std::vector<std::vector<int>> all_ships, std::vector<std::vector<int>> dead_ships);
};
int localTwoPlayersGame();
int localVersusComputerGame();
int menu(int state);
int choice(std::string text, std::vector<std::string> str);

int multiplayer();
int multiplayerGame(Game*& game1);
int client(Game*& game1);
int server(Game*& game1);