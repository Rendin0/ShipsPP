#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>

class Taken
{
	std::vector<int> taken_field;
	size_t zero_index;
	int moves;
	int size;
	std::vector<int> taken_win1;
	std::vector<int> taken_win2;
public:
	Taken(std::vector<int> starting_taken);
	Taken(int starting_size);
	void visual();
	std::vector<int> getField();
	void move(int move_dirrection);
	void playerMove();
	int getMoves();
	std::vector<int> getWin1();
	std::vector<int> getWin2();


};
void getANum(std::string str, int& num);
void fieldCreate(std::vector<int>& field);
void emptyField(std::vector<int> field, int index);
