#include "header.h"

int menu(int state)
{
	system("cls");
	std::cout << "\tShips ++\n" << (state ? "Press any key to start!" : "-----------------------");
	if (_kbhit())
	{
		_getch();
		system("cls");
		int key = choice("Choose game mode.", { "Local two players","Local vs Computer","Multiplayer","Exit" });

		switch (key)
		{
		case 0:
			localTwoPlayersGame();
			return 1;
		case 1:
			localVersusComputerGame();
			return 1;
		case 2:
			multiplayer();
			return 1;
		case 3:
			return 0;
		default:
			return 1;
		}

	}

	return 1;
}

int choice(std::string text, std::vector<std::string> str)
{
	int cho = 0;
	system("cls");
	std::cout << text << std::endl << std::endl;
	for (size_t i = 0; i < str.size(); i++)
	{
		std::cout << (i == cho ? "\u001b[44m" : "") << str.at(i) << (i == cho ? "\u001b[0m" : "") << std::endl;
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			switch (key)
			{
			case 72:
			{
				cho = (cho - 1) < 0 ? str.size() - 1 : cho - 1;
				break;
			}
			case 80:
			{
				cho = (cho + 1) % str.size();
				break;
			}
			case 13:
			{
				std::thread([]() {PlaySound(L"sounds/confirm.wav", NULL, SND_ASYNC); }).join();
				return cho;
			}
			default:
				break;
			}
			//system("cls");
			printf("\x1b[H");
			std::thread([]() {PlaySound(L"sounds/move.wav", NULL, SND_ASYNC); }).join();
			std::cout << text << std::endl << std::endl;
			for (size_t i = 0; i < str.size(); i++)
			{
				std::cout << (i == cho ? "\u001b[44m" : "") << str.at(i) << (i == cho ? "\u001b[0m" : "") << std::endl;
			}
		}
	}
}
