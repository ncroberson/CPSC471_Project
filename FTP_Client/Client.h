#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "21000"

namespace Domain 
{
	class Client 
	{
	public:
		Client();
		int init();
		//int resolve(std::string host, std::string port);
		int resolve_client();
		int resolve_server(std::string host, std::string port);
		int cli_connect();
		int data_connect();
		bool senddata(void *buf, int buflen);
		bool sendfile(FILE *f);
		bool sendlong(long value);
		bool readdata(void *buf, int buflen);
		bool readlong(long *value);
		bool readfile(FILE *f);
		bool sendcommand(std::string command);
		int listen_for_data();
		bool recieveresponse();
		bool recievedata();
		void clean_up();
	private:
		SOCKET control_sock;
		SOCKET data_sock;
		SOCKET ldata_sock;
		WSADATA cli_wsa;
		struct addrinfo *result,
			*ptr,
			hints;
		int iResult;
	};

}


#endif
