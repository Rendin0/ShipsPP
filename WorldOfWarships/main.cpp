#include "header.h"

int main()
{
	std::cout << "\u001b[?25l";
	srand(time(0));

	int state = 1;
	while (menu(state))
	{
		Sleep(500);
		state = !state;
	}
	std::cout << "Bye. Have a nice day!";

	return 0;
}