#ifndef MAIN_CLIENT
#define MAIN_CLIENT

#undef UNICODE

#include <stdlib.h>
#include <stdio.h>
#include "UI_console.h"
#include "Server.h"



int __cdecl main(int argc, char **argv) 
{
	Domain::Server * main_server = new Domain::Server();
	UI_Server::UI_console * main_ui = new UI_Server::UI_console(main_server);
	
	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s port\n", argv[0]);
		return 1;
	}
	
	main_ui->setparams(argv[1]);
	
	main_ui->mainloop();
    

	delete main_ui;
	delete main_server;
    return 0;
}
#endif