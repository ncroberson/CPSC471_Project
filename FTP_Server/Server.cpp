#include "Server.h"
#include <iostream>

Domain::Server::Server()
{
	listen_sock = INVALID_SOCKET;
	client_sock = INVALID_SOCKET;
	data_sock = INVALID_SOCKET;
	client_addr.sin_family = AF_INET;
	this->init();
	result = NULL;
	ptr = NULL;
	close_connection = false;
}

int Domain::Server::init()
{// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &serv_wsa);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	close_connection = false;
	return 0;
}

bool Domain::Server::senddata(void * buf, int buflen)
{
	return false;
}

bool Domain::Server::sendfile(FILE * f)
{
	return false;
}

bool Domain::Server::sendlong(long value)
{
	return false;
}

bool Domain::Server::readdata(void *buf, int buflen)
{
	unsigned char *pbuf = (unsigned char *)buf;

	while (buflen > 0)
	{
		int num = recv(data_sock, reinterpret_cast<char*>(pbuf), buflen, 0);
		if (num == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// optional: use select() to check for timeout to fail the read
				continue;
			}
			return false;
		}
		else if (num == 0)
			return false;

		pbuf += num;
		buflen -= num;
	}

	return true;
}

bool Domain::Server::readlong(long *value)
{
	if (!readdata(value, sizeof(value)))
		return false;
	*value = ntohl(*value);
	return true;
}

bool Domain::Server::readfile(FILE *f)
{
	long filesize;
	if (!readlong(&filesize))
		return false;
	if (filesize > 0)
	{
		char buffer[1024];
		do
		{
			int num = min(filesize, sizeof(buffer));
			if (!readdata(buffer, num))
				return false;
			int offset = 0;
			do
			{
				size_t written = fwrite(&buffer[offset], 1, num - offset, f);
				if (written < 1)
					return false;
				offset += written;
			} while (offset < num);
			filesize -= num;
		} while (filesize > 0);
	}
	return true;
}

int Domain::Server::resolve_server()
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Domain::Server::resolve_client()
{
	return 0;
}

int Domain::Server::control_connect()
{

	addrlen = sizeof(client_addr);

	client_sock = accept(listen_sock, (sockaddr*)&client_addr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}
	return 0;
}

std::string Domain::Server::get_command()
{
	std::string s = "";
	int getResult = 0;
	char buf[128];
	if ((getResult = recv(client_sock, buf, sizeof(buf), 0)) > 0) 
	{
		s.append(buf, buf + getResult);
	}
	else if (getResult == SOCKET_ERROR) 
	{
		close_connection = true;
	}
	
	return s;

}

int Domain::Server::parse_command(std::string pcommand)
{	
	if (pcommand == "") return 1;
	std::cout << "Command recieved: " << pcommand << "\n";
	return 0;
}

int Domain::Server::data_connect()
{
	return 0;
}

int Domain::Server::listen_for_conn()
{
	listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(listen_sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}

	return 0;
}

std::string Domain::Server::get_client_ip()
{
	char str[INET_ADDRSTRLEN];
	return std::string(inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN));
}

int Domain::Server::clean_up()
{
	closesocket(listen_sock);
	// shutdown the connection since we're done
	iResult = shutdown(client_sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(client_sock);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(client_sock);
	WSACleanup();
	return 0;
}

bool Domain::Server::getCloseConn()
{
	return close_connection;
}
