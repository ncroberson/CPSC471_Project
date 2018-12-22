#include "UI_console.h"
#include <iostream>
#include <algorithm>

UI_Client::UI_console::UI_console(): host_name(""), port(0), quit_loop(false) {}

UI_Client::UI_console::UI_console(Domain::Client * cli): host_name(""), port(""), quit_loop(false)
{
	client = cli;
}

void UI_Client::UI_console::mainloop()
{
	//init connect_sock, resolve hostname nad port, try to connect to server
	if (client->init() || client->resolve(host_name, port) || client->cli_connect())
	{	//if any return non zero, print error and return;
		printf_s("Connection could not be established, please try again...");
		std::cin.get();
		return;
	}
	else 
	{
		printf_s("Connection to server successfully established!\n");
	}

	while (!quit_loop) 
	{
		prompt();
	}
}

void UI_Client::UI_console::prompt()
{
	std::cout << "ftp> ";
	size_t command_end = 0;
	std::string input;
	std::getline(std::cin, input);
	std::string command = input;
	if (get_command(command_end, command))
	{
		if (command == "get" || command == "put") 
		{
			std::string fname;
			if (!get_fname(command_end, input, fname))
			{
				std::cout << "\nMissing or invalid filename. Please try again...\n";
				std::cin.get();
				return;
			}
			run_command(command, fname);
			return;
		}
		run_command(command);
		return;
	} 
	else
	{
		std::cout << "\nInvalid command, for usage type command \"help\"...";
		std::cin.get();
	
	}
}

bool UI_Client::UI_console::get_command(size_t &end, std::string &command_str)
{
	std::string input;
	std::string delim = " ";

	if (command_str == "") return false;
	size_t begin = command_str.find_first_not_of(delim, 0);
	command_str = command_str.substr(begin, command_str.length());
	end = command_str.find(delim);
	if (end == std::string::npos) 
	{
		end = command_str.length();
	}
	command_str = command_str.substr(0, end);
	end = end + begin;
	return true;
		
	
}

bool UI_Client::UI_console::get_fname(size_t start, std::string input, std::string &fname)
{
	size_t end  = input.length()-1;
	if (end <= start) 
	{
		return false;
	}
	else 
	{
		fname = input.substr(start, end);
		remove_space(fname);
		return true;
	}
}

void UI_Client::UI_console::remove_space(std::string & text)
{
	text.erase(std::remove(text.begin(), text.end(), ' '), text.end());
}

void UI_Client::UI_console::run_command(std::string command, std::string fname)
{
	bool success = false;
	std::string message = "";
	if (command == "get") 
	{
		success = get(fname, message);
	}
	else if (command == "put") 
	{
		success = put(fname, message);
	}
	else if (command == "ls") 
	{
		success = list(message);
	}
	else if (command == "quit") 
	{
		success = quit(message);
	}
	else if (command == "help") 
	{
		success = true;
		message  = "Commands:\n\n";
		message += "Retrieve file from server:  get <filename>\n";
        message += "      Send file to server:  put <filename>\n";
		message += "     List files on server:  ls\n";
		message += "                View help:  help\n";
		message += "                     Quit:  quit\n";
	}
	else 
	{
		success = false;
		message = "Invalid command, for usage type command \"help\"...";
	}

	if (!success) 
	{
		std::cout << "Command failed:\n";
	}
	std::cout << message << "\n";
}

bool UI_Client::UI_console::get(std::string fname, std::string & message)
{
	std::string command = "get|" + fname;
	if (client->sendcommand(command))
	{
		message = "get succeeded.";
		return true;
	}
	else
	{
		message = "get failed.";
		return false;
	}
}

bool UI_Client::UI_console::put(std::string fname, std::string & message)
{
	std::string command = "put|" + fname;
	if (client->sendcommand(command)) 
	{
		message = "put succeeded.";
		return true;
	}
	else 
	{
		message = "put failed.";
		return false;
	}
}

bool UI_Client::UI_console::list(std::string & message)
{
	std::string command = "list";
	if (client->sendcommand(command))
	{
		message = "list succeeded.";
		return true;
	}
	else
	{
		message = "list failed.";
		return false;
	}
}

bool UI_Client::UI_console::quit(std::string &message)
{
	quit_loop = true;
	std::string command = "quit";
	if (client->sendcommand(command))
	{
		message = "quit succeeded.";
		return true;
	}
	else
	{
		message = "quit failed.";
		return false;
	}
}

void UI_Client::UI_console::set_params(std::string h, std::string p)
{
	host_name = h;
	port = p;
}

	
