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
	
	while (!quit_loop) 
	{
		server->init();
		server->resolve_server();
		server->listen_for_conn();
		std::cout << "Waiting for connections...\n";
		server->control_connect();
		std::cout << "Client connected from IP: " << server->get_client_ip() << "\n";
		while (!server->getCloseConn()) 
		{
			server->init();
			//get command, sent from client
			std::cout << "Awaiting commands...\n";
			std::string command = server->get_command();
			//parse command and perform action
			server->parse_command(command);
		}
		server->clean_up();
		
	}
}