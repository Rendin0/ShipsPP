#include "header.h"

Game::Game()
{
	turn = 0;
}

void Game::fieldsPrint(std::vector<int> point)
{
	std::vector<std::vector<int>> field1 = (turn ? player2.getField() : player1.getField());
	std::vector<std::vector<int>> field2 = (turn ? player1.getField() : player2.getField());
	std::cout << "Player " << turn + 1 << std::endl;
	fieldPrint(field1, false, { -1, -1 });
	std::cout << "\n\n\n";
	std::cout << "Player " << (turn ? "1" : "2") << std::endl;
	fieldPrint(field2, true, point);
}

void Game::attack()
{
	std::vector<int> point(2, 0);
	system("cls");
	fieldsPrint(point);

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

			fieldsPrint(point);

			if (key == 13)
			{
				std::vector<std::vector<int>> enemy_field = (turn ? player1.getField() : player2.getField());

				switch (enemy_field.at(point.at(0)).at(point.at(1)))
				{
				case 0:
					std::thread([]() {PlaySound(L"sounds/miss.wav", NULL, SND_ASYNC); }).join();

					(turn ? player1.setPoint(point, 3) : player2.setPoint(point, 3));
					//system("cls");
					printf("\x1b[H");

					fieldsPrint(point);
					break_point = true;
					break;
				case 1:
					std::thread([]() {PlaySound(L"sounds/hit.wav", NULL, SND_ASYNC); }).join();

					(turn ? player1.setPoint(point, 2) : player2.setPoint(point, 2));
					//system("cls");
					printf("\x1b[H");

					fieldsPrint(point);
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
	int state = 0;

	if (!player1.getState())
	{
		state = 2;
		return state;
	}
	if (!player2.getState())
	{
		state = 1;
		return state;
	}

	return state;
}

void Game::changeTurn()
{

	std::cout << "Next turn!\n";
	system("pause");

	turn = !turn;
}

int localGame()
{
	Game game1;

	while (true)
	{
		game1.attack();

		if (game1.getState() > 0)
		{
			break;
		}

		game1.changeTurn();
	}

	system("cls");
	game1.fieldsPrint({ -1, -1 });
	std::cout << "\n\nPlayer " << game1.getState() << " wins!" << std::endl;
	system("pause");


	return game1.getState();
}