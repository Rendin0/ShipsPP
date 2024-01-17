#include "header.h"

void Game::computerAttack(bool& fog_of_war)
{
	int false_turn = (is_there_a_player ? 0 : turn);

	std::vector<int> null_point(2, -1);
	std::vector<int> point(2, 4);
	system("cls");
	fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));
	bool reverse = false;
	bool break_point = false;

	Player*& curr_cpu = (false_turn ? player1 : player2);
	Player*& curr_enemy = (false_turn ? player2 : player1);

	while (!break_point)
	{
		Sleep(300);
		srand(time(0));

		std::vector<int> ai_point(2, 4);

		if (!curr_cpu->cpu_pointers.empty())
		{
			ai_point.at(0) = curr_cpu->cpu_pointers.at(curr_cpu->cpu_pointers.size() - 1).at(0);
			ai_point.at(1) = curr_cpu->cpu_pointers.at(curr_cpu->cpu_pointers.size() - 1).at(1);

			if (curr_cpu->cpu_dir != -1)
			{
				if (reverse)
				{
					if (ai_point.at(!curr_cpu->cpu_dir) == 0)
					{
						curr_cpu->cpu_pointers.clear();
						reverse = false;
						continue;
					}

					if (curr_enemy->getField().at(ai_point.at(0) - (1 * curr_cpu->cpu_dir)).at(ai_point.at(1) - (1 * !curr_cpu->cpu_dir)) != 3)
					{
						ai_point.at(!curr_cpu->cpu_dir)--;
						curr_cpu->cpu_pointers.push_back({ ai_point.at(0), ai_point.at(1) });
					}
					else
					{
						curr_cpu->cpu_pointers.clear();
						reverse = false;
						continue;
					}
				}
				else
				{
					if (ai_point.at(!curr_cpu->cpu_dir) == 9)
					{
						reverse = true;
						continue;
					}

					if (curr_enemy->getField().at(ai_point.at(0) + (1 * curr_cpu->cpu_dir)).at(ai_point.at(1) + (1 * !curr_cpu->cpu_dir)) != 3)
					{
						ai_point.at(!curr_cpu->cpu_dir)++;
						if (curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1)) != 0)
							curr_cpu->cpu_pointers.push_back({ ai_point.at(0), ai_point.at(1) });
					}
					else
					{
						reverse = true;
					}
				}
			}
			else
			{
				if (curr_cpu->cpu_pointers.size() > 1)
				{
					curr_cpu->cpu_dir = abs(curr_cpu->cpu_pointers.at(0).at(0) - curr_cpu->cpu_pointers.at(1).at(0));
					continue;
				}
				if (ai_point.at(1) < 9 && (curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1) + 1) != 2 && curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1) + 1) != 3))
					ai_point.at(1)++;
				else if (ai_point.at(1) > 0 && (curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1) - 1) != 2 && curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1) - 1) != 3))
					ai_point.at(1)--;
				else if (ai_point.at(0) < 9 && (curr_enemy->getField().at(ai_point.at(0) + 1).at(ai_point.at(1)) != 2 && curr_enemy->getField().at(ai_point.at(0) + 1).at(ai_point.at(1)) != 3))
					ai_point.at(0)++;
				else if (ai_point.at(0) > 0 && (curr_enemy->getField().at(ai_point.at(0) - 1).at(ai_point.at(1)) != 2 && curr_enemy->getField().at(ai_point.at(0) - 1).at(ai_point.at(1)) != 3))
					ai_point.at(0)--;
				else
				{
					curr_cpu->cpu_pointers.clear();
					continue;
				}

			}
		}
		else
		{
			curr_cpu->cpu_dir = -1;
			ai_point.at(0) = rand() % 10;
			ai_point.at(1) = rand() % 10;
			if ((ai_point.at(0) + ai_point.at(1)) % (1 + ((rand() % 20) > 1)) == curr_cpu->cpu_rand_mod)
			{
				ai_point.at(0) = (rand() % 10);
				ai_point.at(1) = (rand() % 10);
			}
			while (curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1)) == 2 || curr_enemy->getField().at(ai_point.at(0)).at(ai_point.at(1)) == 3)
			{
				ai_point.at(0) = (rand() % 10);
				ai_point.at(1) = (rand() % 10);
				if ((ai_point.at(0) + ai_point.at(1)) % (1 + ((rand() % 20) > 1)) == curr_cpu->cpu_rand_mod)
				{
					ai_point.at(0) = (rand() % 10);
					ai_point.at(1) = (rand() % 10);
				}
			}
		}

		while (point.at(0) != ai_point.at(0))
		{
			Sleep(45);
			point.at(0) < ai_point.at(0) ? point.at(0)++ : point.at(0)--;

			//system("cls");
			printf("\x1b[H");

			fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));
		}

		while (point.at(1) != ai_point.at(1))
		{
			Sleep(45);
			point.at(1) < ai_point.at(1) ? point.at(1)++ : point.at(1)--;

			//system("cls");
			printf("\x1b[H");

			fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));
		}


		//system("cls");
		printf("\x1b[H");

		fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));



		std::vector<std::vector<int>> enemy_field = curr_enemy->getField();

		switch (enemy_field.at(point.at(0)).at(point.at(1)))
		{
		case 0:
			std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

			curr_enemy->setPoint(point, 3);
			//system("cls");
			printf("\x1b[H");

			fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));
			break_point = true;
			break;
		case 1:
			std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();
			Sleep(400);

			curr_enemy->setPoint(point, 2);
			//system("cls");
			printf("\x1b[H");

			curr_cpu->cpu_pointers.push_back(point);

			fieldsPrint((false_turn ? null_point : point), (false_turn ? point : null_point), fog_of_war, (is_there_a_player ? false : fog_of_war));

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

void chucklenuts(Game& game1, std::thread& thr)
{
	std::thread([]() {PlaySound(L"sounds/tfc.wav", NULL, SND_ASYNC); }).join();
	Sleep(1250);
	system("cls");
	std::cout << ("\u001b[48;5;15m                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ");
	Sleep(4500);
	std::cout << "\u001b[0m";
	game1.endGame();
	thr.join();
}

std::vector<std::vector<std::vector<int>>> fieldCreateCpu()
{
	std::cout << "Creating field for computer...";
	Sleep(1000);
	std::vector<std::vector<std::vector<int>>> the_answer(2);
	std::vector<std::vector<int>> field(10, std::vector<int>(10, 0));
	std::vector<std::vector<int>> tmp_field(10, std::vector<int>(10, 0));
	const std::vector<int> ships{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

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

int localVersusComputerGame()
{
	Game game1(1);
	bool fog_of_war = true;
	while (true)
	{
		game1.attack();

		if (game1.getState() > 0)
		{
			break;
		}


		game1.computerAttack(fog_of_war);

		if (game1.getState() > 0)
		{
			break;
		}
	}

	if (game1.getState() == 1)
		std::thread([]() {PlaySound(L"sounds/win.wav", NULL, SND_ASYNC); }).join();
	else
		std::thread([]() {PlaySound(L"sounds/lose.wav", NULL, SND_ASYNC); }).join();


	system("cls");
	game1.fieldsPrint({ -1, -1 }, { -1, -1 }, false, false);
	std::cout << "\n\n" << (game1.getState() == 1 ? "Player " : "Computer ") << " wins!" << std::endl;
	system("pause");

	return game1.getState();
}

void fogOfWarToggle(bool& fog_of_war, Game& game1)
{
	while (true)
	{
		if (game1.getState() > 0)
			break;
		if (_kbhit())
		{
			int key = _getch();
			if (key == 102)
				fog_of_war = !fog_of_war;
		}
	}
}

int computerVersusComputer()
{
	bool fog_of_war = true;

	int random = 1200;

	Game game1(3);

	std::thread tggl(fogOfWarToggle, std::ref(fog_of_war), std::ref(game1));

	while (true)
	{
		game1.computerAttack(fog_of_war);

		if (game1.getState() > 0)
		{
			break;
		}

		if (!(rand() % random))
		{
			chucklenuts(game1, tggl);
			return -1;
		}
		game1.changeTurn(true);

		game1.computerAttack(fog_of_war);

		if (game1.getState() > 0)
		{
			break;
		}

		if (!(rand() % random))
		{
			chucklenuts(game1, tggl);
			return -1;
		}
		game1.changeTurn(true);
	}

	tggl.join();
	system("cls");
	game1.fieldsPrint({ -1, -1 }, { -1, -1 }, false, false);
	std::cout << "\n\n" << (game1.getState() == 1 ? "Computer 1 " : "Computer 2 ") << " wins!" << std::endl;
	system("pause");

	return game1.getState();
}