#include "header.h"
#include <ctime>

int main()
{
	srand(time(0));

	int choice = 0;
	int size = 0;
	getANum("Choose size (3, 4): ", size);
	getANum("1. Random\n2. Manual\nChoice: ", choice);

	Taken taken(size);

	if (choice == 2)
	{
		std::vector<int> field_tmp(size * size, 0);
		fieldCreate(field_tmp);
		Taken taken_tmp(field_tmp);
		taken = taken_tmp;
	}


	const std::vector<int> win1 = taken.getWin1();
	const std::vector<int> win2 = taken.getWin2();


	while (true)
	{
		taken.visual();
		if (taken.getField() == win1 || taken.getField() == win2)
		{
			std::cout << "\n\nYou win! Your moves: " << taken.getMoves() << std::endl;
			break;
		}
		taken.playerMove();
	}

	system("pause");

	return 0;
}