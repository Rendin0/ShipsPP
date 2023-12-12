#include "header.h"

Taken::Taken(int starting_size)
{
	moves = 0;
	size = starting_size;
	for (int i = 1; i < (size * size) + 1; i++)
	{
		taken_win2.push_back(i - 1);
		taken_win1.push_back(i % (size * size));
	}

	for (size_t i = 0; i < size * size;)
	{
		bool unique = true;
		int num = rand() % (size * size);
		for (size_t j = 0; j < taken_field.size(); j++)
		{
			if (taken_field.at(j) == num)
			{
				unique = false;
				break;
			}
		}
		if (unique)
		{
			taken_field.push_back(num);
			i++;
		}
	}

	for (size_t i = 0; i < taken_field.size(); i++)
	{
		if (taken_field.at(i) == 0)
		{
			zero_index = i;
			break;
		}
	}

}

Taken::Taken(std::vector<int> starting_taken)
{
	moves = 0;
	size = sqrt(starting_taken.size());
	for	(int i = 1; i < (size * size) + 1; i++)
	{
		taken_win2.push_back(i - 1);
		taken_win1.push_back(i % (size * size));
	}

	for (size_t i = 0; i < starting_taken.size(); i++)
	{
		if (starting_taken.at(i) == 0)
		{
			zero_index = i;
			break;
		}
	}

	taken_field = starting_taken;
}

void Taken::visual()
{
	system("cls");
	std::cout << "Current taken field!";
	for (size_t i = 0; i < taken_field.size(); i++)
	{
		if (i % size == 0)
		{
			std::cout << "\n\n";
		}
		if (taken_field.at(i) != 0)
		{
			std::cout << "[" << taken_field.at(i) << "]";
		}
		std::cout << '\t';
	}
}

void Taken::move(int move_dirrection)
{
	const int moving_to = zero_index + move_dirrection;
	if (moving_to % size >= 0 && moving_to < taken_field.size() && !(moving_to % size == size - 1 && move_dirrection == -1) && !(moving_to % size == 0 && move_dirrection == 1))
	{
		taken_field.at(zero_index) = taken_field.at(moving_to);
		taken_field.at(moving_to) = 0;
		zero_index = moving_to;
		moves++;
	}
}

void Taken::playerMove()
{
	_getch();
	int key = _getch();
	switch (key)
	{
	case 72:
		move(0 - size);
		break;
	case 75:
		move(-1);
		break;
	case 77:
		move(1);
		break;
	case 80:
		move(size);
		break;
	}
}

std::vector<int> Taken::getField()
{
	return taken_field;
}

int Taken::getMoves()
{
	return moves;
}

std::vector<int> Taken::getWin1()
{
	return taken_win1;
}

std::vector<int> Taken::getWin2()
{
	return taken_win2;
}

void getANum(std::string str, int& num)
{
	std::cout << str;
	std::cin >> num;
}

void fieldCreate(std::vector<int>& field)
{
	int index = 0;

	system("cls");
	emptyField(field, index);

	for (size_t i = 1; i < field.size();)
	{
		if (_kbhit())
		{
			int key = _getch();
			switch (key)
			{
			case 72:
				if (index > static_cast<int>(sqrt(field.size())) - 1)
				{
					index -= static_cast<int>(sqrt(field.size()));
				}
				break;
			case 75:
				if (index > 0)
				{
					index--;
				}
				break;
			case 77:
				index = (index + 1) % field.size();
				break;
			case 80:
				index = (index + static_cast<int>(sqrt(field.size()))) % field.size();
				break;
			case 13:
				if (field.at(index) == 0)
				{
					field.at(index) = i;
					i++;
				}
				break;
			}
			system("cls");
			emptyField(field, index);
		}
		
	}
}

void emptyField(std::vector<int> field, int index)
{
	for (size_t i = 0; i < field.size(); i++)
	{
		std::cout << "[" << (i == index ? " " : "") << (field.at(i) != 0 ? std::to_string(field.at(i)) : "") << (i == index ? " " : "") << "]" << (i % static_cast<int>(sqrt(field.size())) == static_cast<int>(sqrt(field.size())) - 1 ? "\n\n" : "\t");
	}
}