#include "header.h"

void Game::computerAttack()
{
	std::vector<int> point(2, 0);
	system("cls");
	fieldsPrint(point, { -1, -1 });
	bool reverse = false;
	bool break_point = false;
	while (!break_point)
	{
		Sleep(300);
		srand(time(0));

		std::vector<int> ai_point(2, 4);

		if (!player2->cpu_pointers.empty())
		{
			ai_point.at(0) = player2->cpu_pointers.at(player2->cpu_pointers.size() - 1).at(0);
			ai_point.at(1) = player2->cpu_pointers.at(player2->cpu_pointers.size() - 1).at(1);

			if (player2->cpu_dir != -1)
			{
				if (reverse)
				{
					if (ai_point.at(!player2->cpu_dir) == 0)
					{
						player2->cpu_pointers.clear();
						continue;
					}

					if (player1->getField().at(ai_point.at(0) - (1 * player2->cpu_dir)).at(ai_point.at(1) - (1 * !player2->cpu_dir)) != 3)
					{
						ai_point.at(!player2->cpu_dir)--;
						player2->cpu_pointers.push_back({ ai_point.at(0), ai_point.at(1) });
					}
					else
					{
						player2->cpu_pointers.clear();
						continue;
					}
				}
				else
				{
					if (ai_point.at(!player2->cpu_dir) == 9)
					{
						reverse = true;
						continue;
					}

					if (player1->getField().at(ai_point.at(0) + (1 * player2->cpu_dir)).at(ai_point.at(1) + (1 * !player2->cpu_dir)) != 3)
					{
						ai_point.at(!player2->cpu_dir)++;
						if (player1->getField().at(ai_point.at(0)).at(ai_point.at(1)) != 3)
							player2->cpu_pointers.push_back({ ai_point.at(0), ai_point.at(1) });
					}
					else
					{
						reverse = true;
					}
				}
			}
			else
			{
				if (player2->cpu_pointers.size() > 1)
				{
					player2->cpu_dir = abs(player2->cpu_pointers.at(0).at(0) - player2->cpu_pointers.at(1).at(0));
					continue;
				}
				if (ai_point.at(1) < 9 && (player1->getField().at(ai_point.at(0)).at(ai_point.at(1) + 1) != 2 && player1->getField().at(ai_point.at(0)).at(ai_point.at(1) + 1) != 3))
					ai_point.at(1)++;
				else if (ai_point.at(1) > 0 && (player1->getField().at(ai_point.at(0)).at(ai_point.at(1) - 1) != 2 && player1->getField().at(ai_point.at(0)).at(ai_point.at(1) - 1) != 3))
					ai_point.at(1)--;
				else if (ai_point.at(0) < 9 && (player1->getField().at(ai_point.at(0) + 1).at(ai_point.at(1)) != 2 && player1->getField().at(ai_point.at(0) + 1).at(ai_point.at(1)) != 3))
					ai_point.at(0)++;
				else if (ai_point.at(0) > 0 && (player1->getField().at(ai_point.at(0) - 1).at(ai_point.at(1)) != 2 && player1->getField().at(ai_point.at(0) - 1).at(ai_point.at(1)) != 3))
					ai_point.at(0)--;
				else
				{
					player2->cpu_pointers.clear();
					continue;
				}
				
			}
		}
		else
		{
			player2->cpu_dir = -1;
			ai_point.at(0) = rand() % 10;
			ai_point.at(1) = rand() % 10;
			while (player1->getField().at(ai_point.at(0)).at(ai_point.at(1)) == 2 || player1->getField().at(ai_point.at(0)).at(ai_point.at(1)) == 3)
			{
				ai_point.at(0) = rand() % 10;
				ai_point.at(1) = rand() % 10;
			}
		}

		while (point.at(0) != ai_point.at(0))
		{
			Sleep(45);
			point.at(0) < ai_point.at(0) ? point.at(0)++ : point.at(0)--;

			//system("cls");
			printf("\x1b[H");

			fieldsPrint(point, { -1, -1 });
		}

		while (point.at(1) != ai_point.at(1))
		{
			Sleep(45);
			point.at(1) < ai_point.at(1) ? point.at(1)++ : point.at(1)--;

			//system("cls");
			printf("\x1b[H");

			fieldsPrint(point, { -1, -1 });
		}


		//system("cls");
		printf("\x1b[H");

		fieldsPrint(point, { -1, -1 });



		std::vector<std::vector<int>> enemy_field = player1->getField();

		switch (enemy_field.at(point.at(0)).at(point.at(1)))
		{
		case 0:
			std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

			player1->setPoint(point, 3);
			//system("cls");
			printf("\x1b[H");

			fieldsPrint(point, { -1, -1 });
			break_point = true;
			break;
		case 1:
			std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();

			player1->setPoint(point, 2);
			//system("cls");
			printf("\x1b[H");

			player2->cpu_pointers.push_back(point);

			fieldsPrint(point, { -1, -1 });

			if (getState() > 0)
			{
				break_point = true;
			}

			break;
		default:
			break;
		}
	}
}

std::vector<std::vector<std::vector<int>>> fieldCreateCpu()
{
	std::cout << "Creating field for computer...";
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

	the_answer.at(0) = field;

	return the_answer;
}

Player::Player(bool cpu)
{
	system("cls");
	std::vector<std::vector<std::vector<int>>> the_answer = fieldCreateCpu();


	field = the_answer.at(0);
	all_ships = the_answer.at(1);
	dead_ships.resize(all_ships.size());

	for (size_t i = 0; i < all_ships.size(); i++)
	{
		dead_ships.at(i).push_back(all_ships.at(i).at(0));
	}
}

int localVersusComputerGame()
{
	Game game1(1);

	while (true)
	{
		game1.attack();

		if (game1.getState() > 0)
		{
			break;
		}

		game1.computerAttack();

		if (game1.getState() > 0)
		{
			break;
		}
	}

	system("cls");
	game1.fieldsPrint({ -1, -1 }, { -1, -1 });
	std::cout << "\n\n" << (game1.getState() == 1 ? "Player " : "Computer ") << " wins!" << std::endl;
	system("pause");

	return game1.getState();
}