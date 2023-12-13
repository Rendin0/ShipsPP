#include "header.h"

int main()
{
	srand(time(0));

	int state = 1;
	while (menu(state))
	{
		Sleep(500);
		state = !state;
	}
	std::cout << "Bye";

	return 0;
}