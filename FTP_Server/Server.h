
#ifndef SERVER_H
#define SERVER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "21"

namespace Domain 
{
	class Server 
	{
	public:
		Server();
		int init();
		bool senddata(void *buf, int buflen);
		bool sendfile(FILE *f);
		bool sendlong(long value);
		bool readdata(void *buf, int buflen);
		bool readlong(long *value);
		bool readfile(FILE *f);
		int resolve_server();
		int resolve_client();
		int control_connect();
		int data_connect();
		std::string get_command();
		int parse_command(std::string);
		int listen_for_conn();
		std::string get_client_ip();
		int clean_up();
		bool getCloseConn();
	private:
		SOCKET listen_sock;
		SOCKET client_sock;
		SOCKET data_sock;
		sockaddr_in client_addr;
		int addrlen;
		WSADATA serv_wsa;
		struct addrinfo *result,
			*ptr,
			hints;
		int iResult;
		bool close_connection;
	};
}




#endif