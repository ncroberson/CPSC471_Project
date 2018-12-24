#include "UI_console.h"


UI_Server::UI_console::UI_console()
{
	server = nullptr;
	quit_loop = false;
}

UI_Server::UI_console::UI_console(Domain::Server* srv) 
{
	server = srv;
}

void UI_Server::UI_console::mainloop() 
{
	server->init();
	server->resolve_server(port);
	server->listen_for_conn();
	while (!quit_loop) 
	{
		std::cout << "Waiting for connections...\n";
		server->control_connect();
		std::cout << "Client connected from IP: " << server->get_client_ip() << "\n";
		while (!server->getCloseConn()) 
		{
			//get command, sent from client
			std::cout << "Awaiting commands...\n";
			std::string command = server->get_command();
			//parse command and perform action
			if(!(command == ""))
				server->parse_command(command);
		}
		server->clean_up_control();
		std::cout << "Connection closed, continue accepting clients? (y/n) : ";
		char option = getchar();
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		if (option == 'n' || option == 'N') quit_loop = true;
		server->setCloseConn(false);
		
	}
	server->clean_up();
}

void UI_Server::UI_console::setparams(std::string p)
{
	port = p;
}
