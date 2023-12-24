#include "header.h"

int client()
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

	return 0;
}

int multiplayer()
{

	int key = choice("Choose multiplayer mode", { "Host", "Client" });
	system("cls");
	switch (key)
	{
	case 0:
	{
		server();
		system("pause");

		break;
	}
	case 1:
	{
		client();
		system("pause");
		
		break;
	}
	default:
		break;
	}

	return 0;
}