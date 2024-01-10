#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <string>
#include <vector>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


std::vector<std::string> getBroadcastAddresses()
{
	int i;

	PMIB_IPADDRTABLE pIPAddrTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	IN_ADDR IPAddr;

	LPVOID lpMsgBuf;

	pIPAddrTable = (MIB_IPADDRTABLE*)MALLOC(sizeof(MIB_IPADDRTABLE));

	if (pIPAddrTable) {
		if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) ==
			ERROR_INSUFFICIENT_BUFFER) {
			FREE(pIPAddrTable);
			pIPAddrTable = (MIB_IPADDRTABLE*)MALLOC(dwSize);

		}
		if (pIPAddrTable == NULL) {
			printf("Memory allocation failed for GetIpAddrTable\n");
			exit(1);
		}
	}
	if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR) {
		printf("GetIpAddrTable failed with error %d\n", dwRetVal);
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),       // Default language
			(LPTSTR)&lpMsgBuf, 0, NULL)) {
			printf("\tError: %s", lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
		exit(1);
	}

	std::vector<std::string> broadcasts;

	for (i = 0; i < ((int)pIPAddrTable->dwNumEntries) - 1; i++) {
		char host_ip[16] = "127.0.0.1";
		char netmask[16] = "255.255.255.255";

		IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[i].dwAddr;
		inet_ntop(AF_INET, (IN_ADDR*)&IPAddr, host_ip, sizeof(host_ip));

		IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[i].dwMask;
		inet_ntop(AF_INET, (IN_ADDR*)&IPAddr, netmask, sizeof(netmask));



		in_addr host, mask, broadcast;
		char broadcast_address[INET_ADDRSTRLEN];

		if (inet_pton(AF_INET, host_ip, &host) == 1 &&
			inet_pton(AF_INET, netmask, &mask) == 1)
			broadcast.s_addr = host.s_addr | ~mask.s_addr;
		else {
			fprintf(stderr, "Failed converting strings to numbers\n");
			exit(1);
		}

		if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) != NULL)
		{
			broadcasts.push_back(broadcast_address);
		}
		else
		{
			fprintf(stderr, "Failed converting number to string\n");
			exit(1);
		}
	}
	if (pIPAddrTable) {
		FREE(pIPAddrTable);
		pIPAddrTable = NULL;
	}

	return broadcasts;
}