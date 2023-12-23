#include "header.h"

int client()
{


	return 0;
}

int server()
{


	return 0;
}

int multiplayer()
{
	std::thread srv(server);
	std::thread cln(client);

	if (choice("Choose multiplayer mode", { "Host", "Client" }) == 1)
		srv.join();


	return 0;
}