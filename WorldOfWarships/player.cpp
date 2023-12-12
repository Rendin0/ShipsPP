#include "header.h"

Player::Player()
{
	system("cls");
	int choice1 = choice("Next player's turn to make a field!", {"Manual", "Random"});
	std::vector<std::vector<std::vector<int>>> the_answer = (choice1 ? fieldCreateRandom() : fieldCreate());
	field = the_answer.at(0);
	all_ships = the_answer.at(1);
	dead_ships.resize(all_ships.size());

	for (size_t i = 0; i < all_ships.size(); i++)
	{
		dead_ships.at(i).push_back(all_ships.at(i).at(0));
	}
}

std::vector<std::vector<std::vector<int>>> fieldCreate()
{
	std::vector<std::vector<std::vector<int>>> the_answer(2);
	std::vector<std::vector<int>> field(10, std::vector<int>(10, 0));
	std::vector<std::vector<int>> tmp_field(10, std::vector<int>(10, 0));
	const std::vector<int> ships{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	for (const auto i : ships)
	{
		int row = 0, line = 0, direction = 0;
		for (int j = 0; j < i; j++)
		{
			field.at(row + (direction * j)).at(line + (!(direction)*j)) = 1;
		}
		system("cls");
		std::cout << "[Arrows] - move.\n[Space] - rotate.\n[Enter] - place ship.\n";
		fieldPrint(field, 0, { -1, -1 });

		while (true)
		{

			if (_kbhit())
			{
				int key = _getch();

				switch (key)
				{
				case 72:
				{
					if (row > 0)
						--row;
					break;
				}
				case 75:
				{
					if (line > 0)
						--line;
					break;
				}
				case 77:
				{
					if (line < (10 - (pow(i, !direction))))
						++line;
					break;
				}
				case 80:
				{
					if (row < (10 - (pow(i, direction))))
						++row;
					break;
				}
				case 32:
				{
					direction = !direction;
					if (row + (direction * i) > 10 || line + (!direction * i) > 10)
					{
						direction = !direction;
					}
					break;
				}
				default:
					break;
				}
				field = tmp_field;
				for (int j = 0; j < i; j++)
				{
					field.at(row + (direction * j)).at(line + (!(direction)*j)) = 1;
				}
				system("cls");
				std::cout << "[Arrows] - move.\n[Space] - rotate.\n[Enter] - place ship.\n";
				fieldPrint(field, 0, { -1, -1 });

				if (key == 13)
				{
					bool aviable = true;

					for (int j = row > 0 ? row - 1 : row; j < row + 1 + (pow(i, direction)) && j < 10; j++)
					{
						for (int k = line > 0 ? line - 1 : line; k < line + 1 + (pow(i, !direction)) && k < 10; k++)
						{
							if (tmp_field.at(j).at(k) == 1)
							{
								aviable = false;
								break;
							}
						}
						if (!aviable)
						{
							break;
						}
					}
					if (aviable)
					{
						std::vector<int> ship;
						ship.push_back(direction);
						for (int j = 0; j < i; j++)
						{
							ship.push_back(row + (direction * j));
							ship.push_back(line + (!(direction)*j));
						}
						the_answer.at(1).push_back(ship);
						tmp_field = field;
						break;
					}
					else
					{
						for (int j = 0; j < i; j++)
						{
							field.at(row + (direction * j)).at(line + (!(direction)*j)) = 4;
						}
						system("cls");
						std::cout << "[Arrows] - move.\n[Space] - rotate.\n[Enter] - place ship.\n";
						fieldPrint(field, 0, { -1, -1 });
						Sleep(300);
						for (int j = 0; j < i; j++)
						{
							field.at(row + (direction * j)).at(line + (!(direction)*j)) = 1;
						}
						system("cls");
						std::cout << "[Arrows] - move.\n[Space] - rotate.\n[Enter] - place ship.\n";
						fieldPrint(field, 0, { -1, -1 });
					}
				}
			}

		}
	}

	the_answer.at(0) = field;

	return the_answer;
}

std::vector<std::vector<std::vector<int>>> fieldCreateRandom()
{
	system("cls");
	std::cout << "Loading...";
	Sleep(1000);
	std::vector<std::vector<std::vector<int>>> the_answer(2);
	std::vector<std::vector<int>> field(10, std::vector<int>(10, 0));
	std::vector<std::vector<int>> tmp_field(10, std::vector<int>(10, 0));
	const std::vector<int> ships{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	for (const auto i : ships)
	{
		int row = 0, line = 0, direction = 0;
		for (int j = 0; j < i; j++)
		{
			field.at(row + (direction * j)).at(line + (!(direction)*j)) = 1;
		}

		for (int o = 0;; o++)
		{
			srand(time(0) + o + rand());

			line = (rand() % static_cast<int>(10 - (pow(i, !direction))));
			row = (rand() % static_cast<int>(10 - (pow(i, direction))));
			int randDirection = rand() % 6;

			int key = rand() % 6;

			if (randDirection == 5)
			{
				direction = !direction;
				if (row + (direction * i) > 10 || line + (!direction * i) > 10)
				{
					direction = !direction;
				}
			}

			field = tmp_field;
			for (int j = 0; j < i; j++)
			{
				field.at(row + (direction * j)).at(line + (!(direction)*j)) = 1;
			}

			if (key == 5)
			{
				bool aviable = true;

				for (int j = row > 0 ? row - 1 : row; j < row + 1 + (pow(i, direction)) && j < 10; j++)
				{
					for (int k = line > 0 ? line - 1 : line; k < line + 1 + (pow(i, !direction)) && k < 10; k++)
					{
						if (tmp_field.at(j).at(k) == 1)
						{
							aviable = false;
							break;
						}
					}
					if (!aviable)
					{
						break;
					}
				}
				if (aviable)
				{
					std::vector<int> ship;
					ship.push_back(direction);
					for (int j = 0; j < i; j++)
					{
						ship.push_back(row + (direction * j));
						ship.push_back(line + (!(direction)*j));
					}
					the_answer.at(1).push_back(ship);
					tmp_field = field;
					break;
				}
			}

		}
	}

	system("cls");
	std::cout << "Your field.\n\n";
	fieldPrint(field, 0, { -1, -1 });
	system("pause");

	the_answer.at(0) = field;

	return the_answer;
}

std::vector<std::vector<int>> Player::getField()
{
	return field;
}

void fieldPrint(std::vector<std::vector<int>> field, bool fogOfWar, std::vector<int> point)
{

	for (size_t i = 0; i < field.size(); i++)
	{
		for (size_t j = 0; j < field.at(i).size(); j++)
		{
			if (i == point.at(0) && j == point.at(1))
				std::cout << "!";
			else
				std::cout << " ";

			switch (field.at(i).at(j))
			{
			case 0:
				std::cout << ".";
				break;
			case 1:
				std::cout << (fogOfWar ? "." : "S");
				break;
			case 2:
				std::cout << "X";
				break;
			case 3:
				std::cout << "O";
				break;
			case 4:
				std::cout << "!";
				break;
			}
			if (i == point.at(0) && j == point.at(1))
				std::cout << "!";
			else
				std::cout << " ";

			std::cout << " ";
		}
		std::cout << "\n";
	}

}

void Player::setPoint(std::vector<int> point, int state)
{
	field.at(point.at(0)).at(point.at(1)) = state;

	if (state == 2)
	{
		for (size_t i = 0; i < all_ships.size(); i++)
		{
			for (size_t j = 1; j < (all_ships.at(i).size() - 1); j += 2)
			{
				if (point.at(0) == all_ships.at(i).at(j) && point.at(1) == all_ships.at(i).at(j + 1))
				{
					dead_ships.at(i).push_back(all_ships.at(i).at(j));
					dead_ships.at(i).push_back(all_ships.at(i).at(j + 1));

					all_ships.at(i).at(j) = -1;
					all_ships.at(i).at(j + 1) = -1;

					bool ship_empty = true;

					for (size_t k = 1; k < all_ships.at(i).size(); k++)
					{
						if (all_ships.at(i).at(k) != -1)
						{
							ship_empty = false;
							break;
						}
					}

					if (ship_empty)
					{
						size_t row = dead_ships.at(i).at(1), line = dead_ships.at(i).at(2);

						for (size_t k = 3; k < dead_ships.at(i).size(); k += 2)
						{
							if (row > dead_ships.at(i).at(k) && line > dead_ships.at(i).at(k + 1))
							{
								row = dead_ships.at(i).at(k);
								line = dead_ships.at(i).at(k + 1);
							}
						}
							shipIsDead(row, line, dead_ships.at(i).at(0), i);
					}
				}
			}
		}
	}
}

void Player::shipIsDead(size_t row, size_t line, int direction, size_t ship_id)
{
	for (int j = row > 0 ? row - 1 : row; j < row + 1 + (pow(dead_ships.at(ship_id).size() / 2, direction)) && j < 10; j++)
	{
		for (int k = line > 0 ? line - 1 : line; k < line + 1 + (pow(dead_ships.at(ship_id).size() / 2, !direction)) && k < 10; k++)
		{
			if (field.at(j).at(k) != 2)
				field.at(j).at(k) = 3;
		}
	}

}

bool Player::getState()
{
	bool state = false;
	for (const auto i : field)
	{
		for (const auto j : i)
		{
			if (j == 1)
			{
				state = true;
				break;
			}
		}
		if (state)
		{
			break;
		}
	}

	return state;
}
