#include "Client.h"

Domain::Client::Client()
{
	control_sock = INVALID_SOCKET;
	data_sock = INVALID_SOCKET;
	ldata_sock = INVALID_SOCKET;
	this->init();
	result = NULL;
	ptr = NULL;
}

int Domain::Client::init()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &cli_wsa);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return 0;
}

//int Domain::Client::resolve(std::string host, std::string port)
//{
//	char* chost;
//	if (host == "") 
//	{
//		chost = NULL;
//	}
//	else chost = const_cast<char *>(host.c_str());
//	std::string res_port = port;
//	if (res_port == "0") res_port = DEFAULT_PORT;
//	// Resolve the server address and port
//	iResult = getaddrinfo(chost, res_port.c_str(), &hints, &result);
//	if (iResult != 0) {
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
//		return 1;
//	}
//
//	//todo delete
//	//print host addresses for debugging purposes
//	for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
//	{
//		char host[256];
//		getnameinfo(ptr->ai_addr, ptr->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
//		printf("%s\n", host);
//	}
//
//	return 0;
//}

int Domain::Client::resolve_client()
{
	iResult = getaddrinfo(NULL, "21000", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Domain::Client::resolve_server(std::string host, std::string port)
{
	// Resolve the server address and port
	iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Domain::Client::cli_connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		control_sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (control_sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(control_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(control_sock);
			control_sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (control_sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	return 0;
}

int Domain::Client::data_connect()
{
	data_sock = accept(ldata_sock, NULL , NULL);
	if (data_sock == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(data_sock);
		WSACleanup();
		return 1;
	}
	return 0;
}

bool Domain::Client::senddata(void * buf, int buflen)
{
	unsigned char *pbuf = (unsigned char *)buf;

	while (buflen > 0)
	{
		int num = send(data_sock, reinterpret_cast<char*>(pbuf), buflen, 0);
		if (num == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// optional: use select() to check for timeout to fail the send
				continue;
			}
			return false;
		}

		pbuf += num;
		buflen -= num;
	}

	return true;
}

bool Domain::Client::sendfile(FILE * f)
{
	fseek(f, 0, SEEK_END);
	long filesize = ftell(f);
	rewind(f);
	if (filesize == EOF)
		return false;
	if (!sendlong(filesize))
		return false;
	if (filesize > 0)
	{
		char buffer[1024];
		do
		{
			size_t num = min(filesize, sizeof(buffer));
			num = fread(buffer, 1, num, f);
			if (num < 1)
				return false;
			if (!senddata(buffer, num))
				return false;
			filesize -= num;
		} while (filesize > 0);
	}
	return true;
}

bool Domain::Client::sendlong(long value)
{
	value = htonl(value);
	return senddata(&value, sizeof(value));
}

bool Domain::Client::readdata(void * buf, int buflen)
{
	return false;
}

bool Domain::Client::readlong(long * value)
{
	return false;
}

bool Domain::Client::readfile(FILE * f)
{
	return false;
}

bool Domain::Client::sendcommand(std::string command) 
{
	if (command.size() > 128) 
	{
		std::cout << "Command too long, try again. (must be 128 characters or less)\n";
		return false;
	}
	int sendResult = 0;

	sendResult = send(control_sock, command.data(), command.size(), 0);
	if (sendResult == SOCKET_ERROR)
		return false;
	return true;
}

int Domain::Client::listen_for_data()
{
	ldata_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ldata_sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ldata_sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ldata_sock);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ldata_sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ldata_sock);
		WSACleanup();
		return 1;
	}

	return 0;
}

bool Domain::Client::recieveresponse()
{
	return false;
}

bool Domain::Client::recievedata()
{
	return false;
}

void Domain::Client::clean_up()
{
	closesocket(control_sock);
	WSACleanup();
}
