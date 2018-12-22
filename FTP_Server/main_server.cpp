#undef UNICODE

#include <stdlib.h>
#include <stdio.h>
#include "UI_console.h"
#include "Server.h"



int __cdecl main(void) 
{
	Domain::Server * main_server = new Domain::Server();
	UI_Server::UI_console * main_ui = new UI_Server::UI_console(main_server);

	main_ui->mainloop();
    

 //   // Receive until the peer shuts down the connection
 //   do {
	//	FILE *filehandle;
	//	fopen_s(&filehandle,"imagefile.jpg", "wb");
	//if (filehandle != NULL)
	//{
	//	bool ok = readfile(ClientSocket, filehandle);
	//	fclose(filehandle);

	//	if (ok)
	//	{
	//		// use file as needed...
	//	}
	//	else
	//		remove("imagefile.jpg");
	//}
							/*
									iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
									if (iResult > 0) {
										printf("Bytes received: %d\n", iResult);

									// Echo the buffer back to the sender
										iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
										if (iSendResult == SOCKET_ERROR) {
											printf("send failed with error: %d\n", WSAGetLastError());
											closesocket(ClientSocket);
											WSACleanup();
											return 1;
										}
										printf("Bytes sent: %d\n", iSendResult);
									}*/
    //    else if (iResult == 0)
    //        printf("Connection closing...\n");
    //    else  {
    //        printf("recv failed with error: %d\n", WSAGetLastError());
    //        closesocket(ClientSocket);
    //        WSACleanup();
    //        return 1;
    //    }

    //} while (iResult > 0);

	delete main_ui;
	delete main_server;
    return 0;
}