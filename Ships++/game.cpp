#include "header.h"

Game::Game(int mode)
{
	switch (mode)
	{
	case 0:
	{
		player1 = new Player;
		player2 = new Player;
		is_there_a_player = true;
		break;
	}
	case 1:
	{
		player1 = new Player;
		player2 = new Player(0);
		is_there_a_player = true;
		break;
	}
	case 2:
	{
		player1 = new Player(1);
		player2 = new Player(1);
		is_there_a_player = true;
		break;
	}
	case 3:
	{
		player1 = new Player(0);
		player2 = new Player(0);
		is_there_a_player = false;
		break;
	}
	default:
	{
		player1 = new Player(1);
		player2 = new Player(1);
		is_there_a_player = true;
		break;
	}
	}

	turn = 0;
}

void Game::endGame()
{
	std::vector<std::vector<int>> all(10, std::vector<int>(10, 3));
	player1->setAll(all, all, all);
}

void Game::fieldsPrint(std::vector<int> point1, std::vector<int> point2, const bool fogOfWar2, const bool fogOfWar1)
{
	std::vector<std::vector<int>> field1;
	std::vector<std::vector<int>> field2;
	if (is_there_a_player)
	{
		field1 = (turn ? player2->getField() : player1->getField());
		field2 = (turn ? player1->getField() : player2->getField());
	}
	else
	{
		field1 = player1->getField();
		field2 = player2->getField();
	}
	std::cout << (is_there_a_player ? "Player " : "Computer ") << (is_there_a_player ? turn + 1 : 1) << std::endl;
	fieldPrint(field1, fogOfWar1, point1);
	std::cout << "\n" << (is_there_a_player ? "" : "Press F to toggle fog of war") << "\n\n";
	std::cout << (is_there_a_player ? (turn ? "Player 1" : "Player 2") : "Computer 2") << std::endl;
	fieldPrint(field2, fogOfWar2, point2);
}

void Game::attack()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	std::vector<int> point(2, 4);
	system("cls");
	fieldsPrint({-1, -1}, point, true, false);

	bool break_point = false;
	while (!break_point)
	{
		if (_kbhit())
		{
			int key = _getch();

			switch (key)
			{
			case 72:
			{
				if (point.at(0) > 0)
					point.at(0)--;
				break;
			}
			case 75:
			{
				if (point.at(1) > 0)
					point.at(1)--;
				break;
			}
			case 77:
			{
				if (point.at(1) < 9)
					point.at(1)++;
				break;
			}
			case 80:
			{
				if (point.at(0) < 9)
					point.at(0)++;
				break;
			}
			}
			//system("cls");
			printf("\x1b[H");

			fieldsPrint({ -1, -1 }, point, true, false);

			if (key == 13)
			{
				std::vector<std::vector<int>> enemy_field = (turn ? player1->getField() : player2->getField());

				switch (enemy_field.at(point.at(0)).at(point.at(1)))
				{
				case 0:
					std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

					(turn ? player1->setPoint(point, 3) : player2->setPoint(point, 3));
					//system("cls");
					printf("\x1b[H");

					fieldsPrint({ -1, -1 }, point, true, false);
					break_point = true;
					break;
				case 1:
					std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();

					(turn ? player1->setPoint(point, 2) : player2->setPoint(point, 2));
					//system("cls");
					printf("\x1b[H");

					fieldsPrint({ -1, -1 }, point, true, false);

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
	}
}

void Game::attackOnline(SOCKET& connection)
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	std::vector<int> point(2, 4);
	system("cls");
	fieldsPrintOnline(point, true, turn);

	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();

			switch (key)
			{
			case 72:
			{
				if (point.at(0) > 0)
					point.at(0)--;
				break;
			}
			case 75:
			{
				if (point.at(1) > 0)
					point.at(1)--;
				break;
			}
			case 77:
			{
				if (point.at(1) < 9)
					point.at(1)++;
				break;
			}
			case 80:
			{
				if (point.at(0) < 9)
					point.at(0)++;
				break;
			}
			}
			//system("cls");
			printf("\x1b[H");

			fieldsPrintOnline(point, true, turn);

			if (key == 13)
			{
				std::vector<std::vector<int>> enemy_field = (main_player_id ? player1->getField() : player2->getField());

				switch (enemy_field.at(point.at(0)).at(point.at(1)))
				{
				case 0:
				{
					std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

					(main_player_id ? player1->setPoint(point, 3) : player2->setPoint(point, 3));
					//system("cls");
					printf("\x1b[H");

					fieldsPrintOnline({-1, -1}, true, !turn);

					Sleep(20);
					send(connection, (char*)&point.at(0), sizeof(int), NULL);
					Sleep(20);

					send(connection, (char*)&point.at(1), sizeof(int), NULL);
					Sleep(20);

					int bf = -1;
					Sleep(20);
					send(connection, (char*)&bf, sizeof(int), NULL);
					Sleep(20);

					send(connection, (char*)&bf, sizeof(int), NULL);
					Sleep(20);

					return;
				}
				case 1:
				{
					std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();

					(main_player_id ? player1->setPoint(point, 2) : player2->setPoint(point, 2));
					//system("cls");
					printf("\x1b[H");

					fieldsPrintOnline(point, true, turn);

					Sleep(20);
					send(connection, (char*)&point.at(0), sizeof(int), NULL);
					Sleep(20);

					send(connection, (char*)&point.at(1), sizeof(int), NULL);
					Sleep(20);

					


					if (getState() > 0)
					{
						int bf = -1;
						Sleep(20);
						send(connection, (char*)&bf, sizeof(int), NULL);
						Sleep(20);

						send(connection, (char*)&bf, sizeof(int), NULL);
						Sleep(20);
						return;
					}
				}
					break;
				default:
					break;
				}
			}
		}
	}

}

int Game::getState()
{
	if (!player1->getState())
		return 2;
	if (!player2->getState())
		return 1;
	return 0;
}

void Game::changeTurn(bool online)
{
	if (!online)
	{
		std::cout << "Next turn!\n";
		system("pause");
	}

	turn = !turn;
}

int localTwoPlayersGame()
{
	Game game1(0);

	while (true)
	{
		game1.attack();

		if (game1.getState() > 0)
		{
			break;
		}

		game1.changeTurn(false);
	}

	std::thread([]() {PlaySound(L"sounds/win.wav", NULL, SND_ASYNC); }).join();

	system("cls");
	game1.fieldsPrint({ -1, -1 }, {-1, -1}, false, false);
	std::cout << "\n\nPlayer " << game1.getState() << " wins!" << std::endl;
	system("pause");


	return game1.getState();
}

void Game::setPlayer(int id, std::vector<std::vector<int>> field, std::vector<std::vector<int>> all_ships, std::vector<std::vector<int>> dead_ships)
{
	if (!id)
	{
		player1 = new Player(field, all_ships, dead_ships);
		return;
	}
	else
	{
		player2 = new Player(field, all_ships, dead_ships);
		return;
	}
}

void Game::setPlayer(int id, bool mode)
{
	if (!id)
	{
		player1 = new Player;
		main_player_id = 0;
		return;
	}
	else
	{
		player2 = new Player;
		main_player_id = 1;
		return;
	}
}

std::vector<std::vector<std::vector<int>>> Game::getPlayer(int id)
{
	if (!id)
		return player1->getAll();
	else
		return player2->getAll();
}

void Game::fieldsPrintOnline(std::vector<int> point, bool fogOfWar, int _turn)
{
	std::vector<std::vector<int>> field1 = (main_player_id ? player2->getField() : player1->getField());
	std::vector<std::vector<int>> field2 = (main_player_id ? player1->getField() : player2->getField());

	std::cout << "Player " << main_player_id + 1 << std::endl;
	fieldPrint(field1, false, {-1, -1});

	std::cout << "\n" << (main_player_id == _turn ? "Your" : "Enemy") << " turn. \n\n";
	std::cout << "Player " << (main_player_id ? "1" : "2") << std::endl;
	fieldPrint(field2, fogOfWar, point);
}

int Game::getTurn()
{
	return turn;
}

int Game::getMainPlayer()
{
	return main_player_id;
}

void Game::attackMainPlayer(std::vector<int> point)
{
	std::vector<std::vector<int>> main_field = (main_player_id ? player2->getField() : player1->getField());

	switch (main_field.at(point.at(0)).at(point.at(1)))
	{
	case 0:
		std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

		(main_player_id ? player2->setPoint(point, 3) : player1->setPoint(point, 3));
		//system("cls");
		printf("\x1b[H");

		fieldsPrintOnline({-1, -1}, true, !main_player_id);
		break;
	case 1:
		std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();

		(main_player_id ? player2->setPoint(point, 2) : player1->setPoint(point, 2));
		//system("cls");
		printf("\x1b[H");

		fieldsPrintOnline({-1, -1}, true, !main_player_id);

		break;
	default:
		break;
	}

}