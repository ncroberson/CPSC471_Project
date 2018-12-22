

#ifndef MAIN_CLIENT
#define MAIN_CLIENT

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "UI_console.h"
#include "Client.h"



int __cdecl main(int argc, char **argv) 
{
	Domain::Client * main_client = new Domain::Client();
	UI_Client::UI_console * main_ui = new UI_Client::UI_console(main_client);
	
	// Validate the parameters
	if (argc != 3) {
		printf("usage: %s server-name port\n", argv[0]);
		return 1;
	}

	main_ui->set_params(argv[1], argv[2]);

	main_ui->mainloop();



    // Send an initial buffer
	/*FILE *filehandle;
	fopen_s(&filehandle,"image.jpg", "rb");
	if (filehandle != NULL)
	{
		sendfile(ConnectSocket, filehandle);
		fclose(filehandle);
	}*/
											/*
											iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
											if (iResult == SOCKET_ERROR) {
												printf("send failed with error: %d\n", WSAGetLastError());
												closesocket(ConnectSocket);
												WSACleanup();
												return 1;
											}*/

											// printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    
	
	/*iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }*/

    // Receive until the peer closes the connection

    /*do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );*/

	//delete ui pointer and client pointer
	delete main_ui;
	delete main_client;
    return 0;
}

#endif