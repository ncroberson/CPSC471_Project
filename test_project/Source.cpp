#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "21000"

int __cdecl main()
{
	std::string host = "DESKTOP-T5BF8HJ";
	std::string port = "20000";

	SOCKET data_sock;
	sockaddr_in client_addr;
	int addrlen;
	WSADATA serv_wsa;
	struct addrinfo *result,
		*ptr,
		hints;
	int iResult;

	
	data_sock = INVALID_SOCKET;
	result = NULL;
	ptr = NULL;

	iResult = WSAStartup(MAKEWORD(2, 2), &serv_wsa);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;



	// Resolve the server address and port
	iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to client
		data_sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (data_sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to client.
		iResult = connect(data_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("connect failed with error: %d\n", WSAGetLastError());
			closesocket(data_sock);
			data_sock = INVALID_SOCKET;
			continue;
		}
		else{
			std::cout << "Connected successfully\n";
		}
		break;
	}

	freeaddrinfo(result);

	if (data_sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}


	return 0;


}