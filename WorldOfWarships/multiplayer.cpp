#include "header.h"
#include "ipfinder.h"

void startupRecv(Game*& game1, SOCKET& connection, int& is_func_ended, int need_plr)
{
	std::vector<std::vector<std::vector<int>>> buff(3);
	int buf_size;

	for (int i = 0; i < 3; i++)
	{
		recv(connection, (char*)&buf_size, sizeof(int), NULL);
		buff.at(i).resize(buf_size);
		for (int j = 0; j < buff.at(i).size(); j++)
		{
			recv(connection, (char*)&buf_size, sizeof(int), NULL);
			buff.at(i).at(j).resize(buf_size);
			for (int k = 0; k < buff.at(i).at(j).size(); k++)
			{
				recv(connection, (char*)&buff.at(i).at(j).at(k), sizeof(int), NULL);
			}
		}
	}

	game1->setPlayer(need_plr, buff.at(0), buff.at(1), buff.at(2));

	is_func_ended = 1;
}

int startupSend(Game*& game1, SOCKET& connection, int player)
{
	std::vector<std::vector<std::vector<int>>> buff = game1->getPlayer(player);
	int buf_size;

	for (int i = 0; i < 3; i++)
	{
		buf_size = buff.at(i).size();
		send(connection, (char*)&buf_size, sizeof(int), NULL);
		Sleep(10);
		for (int j = 0; j < buff.at(i).size(); j++)
		{
			buf_size = buff.at(i).at(j).size();
			send(connection, (char*)&buf_size, sizeof(int), NULL);
			Sleep(10);
			for (int k = 0; k < buff.at(i).at(j).size(); k++)
			{
				send(connection, (char*)&buff.at(i).at(j).at(k), sizeof(int), NULL);
				Sleep(10);
			}
		}
	}

	return 0;
}

void serverChoiceRecv(std::vector<sockaddr_in>& servers, std::vector<std::string>& host_names, SOCKET& broadcast_socket, bool& choosing)
{
	while (choosing)
	{
		Sleep(10);
		char buf[128];
		sockaddr_in server_info;
		int server_info_size = sizeof(server_info);
		recvfrom(broadcast_socket, buf, sizeof(buf), NULL, (sockaddr*)&server_info, &server_info_size);

		if (buf != "ping1337wowkek")
		{
			servers.push_back(server_info);
			host_names.push_back(buf);
		}
	}
}

void serverChoiceMenu(std::vector<std::string>& host_names, int& index, bool& choosing, SOCKET& broadcast_socket, sockaddr_in& broadcast, std::vector<sockaddr_in>& servers,
	SOCKET& main_socket, bool& connected)
{
	sockaddr_in direct;
	ZeroMemory(&direct, sizeof(direct));

	direct.sin_family = AF_INET;
	direct.sin_port = htons(2345);

	system("cls");
	char msg[32] = "ping1337wowkek";
	sendto(broadcast_socket, msg, sizeof(msg), NULL, (sockaddr*)&broadcast, sizeof(broadcast));
	std::cout << "Servers (R to reload, F to direct connect): " << std::endl << std::endl;
	for (size_t i = 0; i < host_names.size(); i++)
	{
		std::cout << (i == index ? "\u001b[44m" : "") << host_names.at(i) << (i == index ? "\u001b[0m" : "") << std::endl;
	}

	while (choosing)
	{
		/*if (index >= host_names.size() && !host_names.empty())
		{
			system("cls");
			index = host_names.size() - 1;
			std::cout << "Servers (R to reload, F to direct connect): " << std::endl << std::endl;
			for (size_t i = 0; i < host_names.size(); i++)
			{
				std::cout << (i == index ? "\u001b[44m" : "") << host_names.at(i) << (i == index ? "\u001b[0m" : "") << std::endl;
			}
		}*/
		//system("cls");
		printf("\x1b[H");
		std::cout << "Servers (R to reload, F to direct connect): " << std::endl << std::endl;
		for (size_t i = 0; i < host_names.size(); i++)
		{
			std::cout << (i == index ? "\u001b[44m" : "") << host_names.at(i) << (i == index ? "\u001b[0m" : "") << std::endl;
		}
		if (_kbhit())
		{
			int key = _getch();
			switch (key)
			{
			case 72:
			{
				if (!host_names.empty())
				{
					std::thread([]() {PlaySound(L"sounds/move.wav", NULL, SND_ASYNC); }).join();
					index = (index - 1) < 0 ? host_names.size() - 1 : index - 1;
				}
				break;
			}
			case 80:
			{
				if (!host_names.empty())
				{
					std::thread([]() {PlaySound(L"sounds/move.wav", NULL, SND_ASYNC); }).join();
					index = (index + 1) % host_names.size();
				}
				break;
			}
			case 13:
			{
				if (!host_names.empty())
				{
					choosing = false;
					std::thread([]() {PlaySound(L"sounds/confirm.wav", NULL, SND_ASYNC); }).join();

					return;
				}
				break;
			}
			case 114:
			{
				system("cls");
				std::cout << "Servers (R to reload, F to direct connect): " << std::endl << std::endl;
				index = 0;
				host_names.clear();
				servers.clear();
				sendto(broadcast_socket, msg, sizeof(msg), NULL, (sockaddr*)&broadcast, sizeof(broadcast));

				break;
			}
			case 102:
			{
				char direct_connection[32];
				system("cls");
				std::cout << "Enter ip (format: 127.0.0.1): ";
				std::cin >> direct_connection;
				in_addr ip_to_num;
				ZeroMemory(&ip_to_num, sizeof(ip_to_num));

				int err_stat = inet_pton(AF_INET, direct_connection, &ip_to_num);
				if (err_stat <= 0)
				{
					std::cout << "Wrong ip format" << std::endl;
					system("pause");
					system("cls");
					break;
				}
				direct.sin_addr = ip_to_num;

				std::cout << "Trying to connect..." << std::endl;
				err_stat = connect(main_socket, (sockaddr*)&direct, sizeof(direct));
				if (err_stat != 0) {
					std::cout << "Server not found. " << std::endl;
					system("cls");
					break;
				}

				if (servers.empty())
					servers.push_back(direct);
				else
					servers.at(0) = direct;
				index = 0;
				choosing = false;
				connected = true;
				servers.at(index).sin_port = htons(30000u);

				return;
			}
			default:
				break;
			}

		}
	}
}

sockaddr_in serverChoice(SOCKET& main_socket, bool& connected)
{
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, NULL);
	if (broadcast_socket == INVALID_SOCKET)
	{
		std::cout << "Error socket init #" << WSAGetLastError() << std::endl;
		closesocket(broadcast_socket);
		WSACleanup();
		exit(1);
	}

	sockaddr_in broadcast_info;
	ZeroMemory(&broadcast_info, sizeof(broadcast_info));

	in_addr ip_to_num;
	int err_stat = inet_pton(AF_INET, "192.168.0.255", &ip_to_num);
	if (err_stat <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		closesocket(broadcast_socket);
		WSACleanup();
		exit(1);
	}

	broadcast_info.sin_addr = ip_to_num;
	broadcast_info.sin_family = AF_INET;
	broadcast_info.sin_port = htons(30000u);

	char bOptVal = TRUE;
	setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &bOptVal, sizeof(bOptVal));

	std::vector<std::string> host_names;
	std::vector<sockaddr_in> servers;

	bool choosing = true;
	int index = 0;

	sockaddr_in isocket_info;
	ZeroMemory(&isocket_info, sizeof(isocket_info));

	isocket_info.sin_addr.s_addr = INADDR_ANY;
	isocket_info.sin_family = AF_INET;
	isocket_info.sin_port = htons(20000u);

	err_stat = bind(broadcast_socket, (sockaddr*)&isocket_info, sizeof(isocket_info));
	if (err_stat != 0)
	{
		std::cout << "Error socket binding server info #" << WSAGetLastError() << std::endl;
		closesocket(broadcast_socket);
		WSACleanup();
		exit(1);
	}

	std::thread srvchrecv(serverChoiceRecv, std::ref(servers), std::ref(host_names), std::ref(broadcast_socket), std::ref(choosing));

	serverChoiceMenu(host_names, index, choosing, broadcast_socket, broadcast_info, servers, main_socket, connected);

	srvchrecv.detach();

	closesocket(broadcast_socket);

	return servers.at(index);
}

void connectionReciver(std::string& host_name, SOCKET& broadcast_socket, bool& connected)
{
	sockaddr_in client_info;
	int client_info_size = sizeof(client_info);

	char buf[32];

	while (!connected)
	{
		recvfrom(broadcast_socket, buf, sizeof(buf), NULL, (sockaddr*)&client_info, &client_info_size);
		Sleep(rand() % 100);
		client_info.sin_family = AF_INET;
		client_info.sin_port = htons(20000u);

		sendto(broadcast_socket, host_name.c_str(), sizeof(host_name), NULL, (sockaddr*)&client_info, client_info_size);
		ZeroMemory(&client_info, client_info_size);
		client_info_size = sizeof(client_info);
	}
}

int client()
{
	WSAData ws_data;
	int err_stat = WSAStartup(MAKEWORD(2, 2), &ws_data);
	if (err_stat != 0)
	{
		std::cout << "Error WinSock version init #" << WSAGetLastError() << std::endl;
		return 1;
	}
	bool connected = false;


	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		std::cout << "Error socket init #" << WSAGetLastError() << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	sockaddr_in server_info = serverChoice(client_socket, connected);
	server_info.sin_port = htons(2345);

	if (!connected)
	{
		err_stat = connect(client_socket, (sockaddr*)&server_info, sizeof(server_info));

		if (err_stat != 0) {
			std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
			closesocket(client_socket);
			WSACleanup();
			return 1;
		}
	}

	Game* game1;

	int is_func_ended = 0;
	int need_plr = 0;

	std::thread strtpRcv(startupRecv, std::ref(game1), std::ref(client_socket), std::ref(is_func_ended), need_plr);

	game1 = new Game(2);
	game1->setPlayer(1, true);

	system("cls");
	std::cout << "Waiting for another player..." << std::endl;

	startupSend(game1, client_socket, 1);

	while (!is_func_ended);

	strtpRcv.join();

	multiplayerGame(game1, client_socket);

	system("pause");

	closesocket(client_socket);
	WSACleanup();

	return 0;
}

int server()
{
	WSAData ws_data;
	int err_stat = WSAStartup(MAKEWORD(2, 2), &ws_data);
	if (err_stat != 0)
	{
		std::cout << "Error WinSock version init #" << WSAGetLastError() << std::endl;
		return 1;
	}

	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		std::cout << "Error socket init #" << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, NULL);
	if (broadcast_socket == INVALID_SOCKET)
	{
		std::cout << "Error socket init #" << WSAGetLastError() << std::endl;
		closesocket(broadcast_socket);
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	std::vector<std::string> ips = ipFinder();
	if (ips.empty())
	{
		std::cout << "No local networks found." << std::endl;
		return 1;
	}

	in_addr ip_to_num;
	if (ips.size() > 1)
	{
		err_stat = inet_pton(AF_INET, ips.at(choice("Choose local network ip", ips)).c_str(), &ip_to_num);
	}
	else
	{
		err_stat = inet_pton(AF_INET, ips.at(0).c_str(), &ip_to_num);
	}


	if (err_stat <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}

	sockaddr_in server_info;
	ZeroMemory(&server_info, sizeof(server_info));

	server_info.sin_addr = ip_to_num;
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(2345);

	err_stat = bind(server_socket, (sockaddr*)&server_info, sizeof(server_info));
	if (err_stat != 0)
	{
		std::cout << "Error socket binding server info #" << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		WSACleanup();
	}

	err_stat = listen(server_socket, SOMAXCONN);
	if (err_stat != 0)
	{
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	system("cls");
	std::string host_name;
	std::cout << "Enter lobby name: ";
	std::getline(std::cin, host_name, '\n');


	sockaddr_in broadcast_info;
	ZeroMemory(&broadcast_info, sizeof(broadcast_info));

	broadcast_info.sin_addr = ip_to_num;
	broadcast_info.sin_family = AF_INET;
	broadcast_info.sin_port = htons(30000u);

	err_stat = bind(broadcast_socket, (sockaddr*)&broadcast_info, sizeof(broadcast_info));
	if (err_stat != 0)
	{
		std::cout << "Error socket binding server info #" << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		closesocket(broadcast_socket);
		WSACleanup();
		return 1;
	}
	bool connected = false;
	std::thread cnnctrecv(connectionReciver, std::ref(host_name), std::ref(broadcast_socket), std::ref(connected));

	system("cls");
	std::cout << "Waiting for client to connect..." << std::endl;

	sockaddr_in client_info;
	ZeroMemory(&client_info, sizeof(client_info));

	int client_info_size = sizeof(client_info);

	SOCKET client_connection = accept(server_socket, (sockaddr*)&client_info, &client_info_size);
	if (client_connection == INVALID_SOCKET)
	{
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		closesocket(client_connection);
		WSACleanup();
		return 1;
	}
	connected = true;
	
	cnnctrecv.detach();
	closesocket(broadcast_socket);
	system("cls");

	Game* game1;

	int is_func_ended = 0;
	int need_plr = 1;

	std::thread strtpRcv(startupRecv, std::ref(game1), std::ref(client_connection), std::ref(is_func_ended), need_plr);

	game1 = new Game(2);
	game1->setPlayer(0, true);


	system("cls");
	std::cout << "Waiting for another player..." << std::endl;

	startupSend(game1, client_connection, 0);

	while (!is_func_ended);

	strtpRcv.join();

	multiplayerGame(game1, client_connection);

	system("pause");

	closesocket(server_socket);
	closesocket(client_connection);
	WSACleanup();

	return 0;
}

int multiplayerGame(Game*& game1, SOCKET& connection)
{
	int main_player = game1->getMainPlayer();

	system("cls");
	game1->fieldsPrintOnline({ -1, -1 }, true, game1->getTurn());

	while (true)
	{
		if (game1->getTurn() == main_player)
		{
			game1->attackOnline(connection);

			game1->changeTurn(true);
		}
		else
		{
			while (true)
			{
				std::vector<int> point(2);
				recv(connection, (char*)&point.at(0), sizeof(int), NULL);
				recv(connection, (char*)&point.at(1), sizeof(int), NULL);

				if (point.at(0) == -1)
					break;

				game1->attackMainPlayer(point);
			}
			game1->changeTurn(true);
		}

		if (game1->getState() > 0)
			break;
	}
	const int state = game1->getState();

	if (state - 1 == main_player)
		std::thread([]() {PlaySound(L"sounds/win.wav", NULL, SND_ASYNC); }).join();
	else
		std::thread([]() {PlaySound(L"sounds/lose.wav", NULL, SND_ASYNC); }).join();


	system("cls");

	game1->fieldsPrintOnline({ -1, -1 }, false, game1->getTurn());

	std::cout << "\nYou " << (state - 1 == main_player ? "win!" : "lose :(") << std::endl;

	return state;
}

int multiplayer()
{
	Game* game1 = nullptr;

	int key = choice("Choose multiplayer mode", { "Host", "Client" });
	system("cls");
	switch (key)
	{
	case 0:
	{
		SetConsoleTitle(L"Sea battle! Server");

		server();

		break;
	}
	case 1:
	{
		SetConsoleTitle(L"Sea battle! Client");

		client();

		break;
	}
	default:
		break;
	}

	return 0;
}