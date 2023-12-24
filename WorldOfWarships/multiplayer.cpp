#include "header.h"

int client(Game*& game1)
{
	WSAData ws_data;
	int err_stat = WSAStartup(MAKEWORD(2, 2), &ws_data);
	if (err_stat != 0)
	{
		std::cout << "Error WinSock version init #" << WSAGetLastError() << std::endl;
		return 1;
	}

	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		std::cout << "Error socket init #" << WSAGetLastError() << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	in_addr ip_to_num;
	err_stat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (err_stat <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	sockaddr_in server_info;

	ZeroMemory(&server_info, sizeof(server_info));

	server_info.sin_family = AF_INET;
	server_info.sin_addr = ip_to_num;
	server_info.sin_port = htons(2345);

	err_stat = connect(client_socket, (sockaddr*)&server_info, sizeof(server_info));

	if (err_stat != 0) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	game1 = new Game(3);

	std::vector<std::vector<std::vector<int>>> buff(3);
	int buf_size;

	for (int i = 0; i < 3; i++)
	{
		recv(client_socket, (char*)&buf_size, sizeof(int), NULL);
		buff.at(i).resize(buf_size);
		for (int j = 0; j < buff.at(i).size(); j++)
		{
			recv(client_socket, (char*)&buf_size, sizeof(int), NULL);
			buff.at(i).at(j).resize(buf_size);
			for (int k = 0; k < buff.at(i).at(j).size(); k++)
			{
				recv(client_socket, (char*)&buff.at(i).at(j).at(k), sizeof(int), NULL);
			}
		}
	}

	game1->setPlayer(0, buff.at(0), buff.at(1), buff.at(2));

	game1->fieldsPrint({-1, -1}, {-1, -1}, 0);

	system("pause");

	closesocket(client_socket);
	WSACleanup();

	return 0;
}

int server(Game*& game1)
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

	in_addr ip_to_num;
	err_stat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
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

	game1 = new Game(2);
	std::vector<std::vector<std::vector<int>>> buff = game1->getPlayer(0);
	int buf_size;

	for (int i = 0; i < 3; i++)
	{
		buf_size = buff.at(i).size();
		send(client_connection, (char*)&buf_size, sizeof(int), NULL);
		Sleep(10);
		for (int j = 0; j < buff.at(i).size(); j++)
		{
			buf_size = buff.at(i).at(j).size();
			send(client_connection, (char*)&buf_size, sizeof(int), NULL);
			Sleep(10);
			for (int k = 0; k < buff.at(i).at(j).size(); k++)
			{
				send(client_connection, (char*)&buff.at(i).at(j).at(k), sizeof(int), NULL);
				Sleep(10);
			}
		}
	}

	closesocket(server_socket);
	closesocket(client_connection);
	WSACleanup();

	return 0;
}

int multiplayerGame(Game*& game1)
{
	while (game1 == nullptr);
	while (!game1->allDone());


	return 0;
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
		std::thread srv(server, std::ref(game1));
		multiplayerGame(game1);
		system("pause");

		srv.join();
		break;
	}
	case 1:
	{
		std::thread cln(client, std::ref(game1));
		multiplayerGame(game1);
		system("pause");

		cln.join();
		break;
	}
	default:
		break;
	}

	return 0;
}