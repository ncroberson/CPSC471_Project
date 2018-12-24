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
	unsigned char *pbuf = (unsigned char *)buf;

	while (buflen > 0)
	{
		int num = send(data_sock, reinterpret_cast<char*>(pbuf), buflen, 0);
		if (num == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}
			return false;
		}

		pbuf += num;
		buflen -= num;
	}

	return true;
}

bool Domain::Server::sendfile(FILE * f)
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

bool Domain::Server::sendlong(long value)
{
	value = htonl(value);
	return senddata(&value, sizeof(value));
}

bool Domain::Server::sendstring(std::string s)
{
	long size = (long)s.size();
	if (size == 0) 
	{
		return false;
	}
	if (!sendlong(size))
	{
		return false;
	}
	if (size > 0)
	{
		int start = 0;
		std::string buffer;
		do 
		{
			
			size_t num = min(size, DEFAULT_BUFLEN);
			buffer = s.substr(start, num);
			if (!senddata(&buffer[0], buffer.size()))
			{
				return false;
			}
			size -= num;
			start += num;

		} while (size > 0);
	}
	return true;
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
		return true;
	}
	return false;
	
}

bool Domain::Server::list()
{
	std::string listing = "";
	std::string path = get_path();
	path += "*";
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile( path.c_str(), &data);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		listing = "No files found.";
	}
	else 
	{
		listing += "\nList of Files\n------------------------------------------\n";
		do {
			listing += data.cFileName;
			listing += "\n";
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	listing += "------------------------------------------\n";
	sendstring(listing);
	return true;
}

bool Domain::Server::get(std::string filename)
{
	std::string path = get_path();
	path += filename;
	FILE *file;
	fopen_s(&file ,path.c_str(), "rb");
	if (file != NULL)
	{
		bool success = sendfile(file);
		fclose(file);
		return success;
	}
	else 
	{
		sendlong(NULL);
		return false;
	}
}

bool Domain::Server::put(std::string filename)
{
	std::string path = get_path();
	path += filename;
	FILE *file;
	fopen_s(&file, path.c_str(), "wb");
	if (file != NULL)
	{
		bool success = readfile(file);
		fclose(file);
		if (!success) 
		{	
			std::string path = get_path();
			path += filename;
			remove(path.c_str());
		}
		return success;
	}
	else return false;
}

std::string Domain::Server::get_path()
{
	std::string path = "";
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	path = std::string(buffer);
	int last = path.find_last_of("\\/");
	path = path.substr(0, last);
	last = path.find_last_of("\\/");
	path = path.substr(0, last);
	path += "\\files\\";
	CreateDirectory(path.c_str(), NULL);
	path += "server\\";
	CreateDirectory(path.c_str(), NULL);
	return path;
}

int Domain::Server::resolve_server(std::string port)
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Domain::Server::resolve_client()
{
	char str[INET_ADDRSTRLEN];
	PCSTR client_ip = inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
	// Resolve the server address and port
	iResult = getaddrinfo(client_ip, "20", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}

int Domain::Server::control_connect()
{

	addrlen = sizeof(client_addr);

	client_sock = accept(listen_sock, (sockaddr*)&client_addr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(client_sock);
		return 1;
	}
	return 0;
}


int Domain::Server::data_connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to client
		data_sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (data_sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			return 1;
		}

		// Connect to client.
		iResult = connect(data_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(data_sock);
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (data_sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		printf("connect failed with error: %d\n", WSAGetLastError());
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
	else
	{
		close_connection = true;
	}
	
	return s;

}

int Domain::Server::parse_command(std::string pcommand)
{	
	std::string filename = "";
	if (pcommand == "") return 1;
	std::cout << "Command recieved: " << pcommand << "\n";
	
	if (pcommand == "list") 
	{
		resolve_client();
		data_connect();
		list();
		clean_up_data();
	}
	else if (pcommand.substr(0, pcommand.find('|')) == "get") 
	{
		filename = pcommand.substr(pcommand.find('|') + 1, pcommand.length() - 1);
		resolve_client();
		data_connect();
		get(filename);
		clean_up_data();
	}
	else if (pcommand.substr(0, pcommand.find('|')) == "put") 
	{
		filename = pcommand.substr(pcommand.find('|') + 1, pcommand.length() - 1);
		resolve_client();
		data_connect();
		put(filename);
		clean_up_data();
	}
	else 
	{
		close_connection = true;
	}
	return 0;
}

int Domain::Server::listen_for_conn()
{
	listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listen_sock);
		close_connection = true;
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(listen_sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_sock);
		close_connection = true;
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
	WSACleanup();
	return 0;
}

int Domain::Server::clean_up_data()
{
	iResult = shutdown(data_sock, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(data_sock);
		return 1;
	}
	closesocket(data_sock);
	return 0;
}

int Domain::Server::clean_up_control()
{
	// shutdown the connection since we're done
	iResult = shutdown(client_sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(client_sock);
		return 1;
	}

	// cleanup
	closesocket(client_sock);
	return 0;
}

bool Domain::Server::getCloseConn()
{
	return close_connection;
}

void Domain::Server::setCloseConn(bool conn)
{
	close_connection = conn;
}
