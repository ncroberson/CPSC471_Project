

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

	delete main_ui;
	delete main_client;
    return 0;
}

#endif