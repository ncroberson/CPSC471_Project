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
	if (client->init() || client->resolve_server(host_name, port) || client->cli_connect())
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
	client->clean_up();
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
		std::cout << "Command failed: ";
	}
	std::cout << message << "\n";
}

bool UI_Client::UI_console::get(std::string fname, std::string & message)
{
	std::string command = "get|" + fname;
	client->resolve_client();
	client->listen_for_data();
	if (client->sendcommand(command))
	{
		client->data_connect();
		long bytes_read;
		std::string path = client->get_path();
		path += fname;
		FILE *file;
		fopen_s(&file, path.c_str(), "wb");
		if (file != NULL) 
		{
			bool success = client->readfile(file, bytes_read);
			fclose(file);
			if (!success)
			{
				std::string path = client->get_path();
				path += fname;
				remove(path.c_str());
				message = "get failed.";
				client->clean_up_data();
				return false;
			}
		}
		else
		{
			message = "get failed.";
			client->clean_up_data();
			return false;
		}
		message = "get succeeded.";
		std::cout << "File: " << fname << std::endl;
		std::cout << "Bytes recieved: " << bytes_read << std::endl;
		client->clean_up_data();
		return true;
	}
	else
	{
		message = "get failed.";
		client->clean_up_data();
		return false;
	}
}

bool UI_Client::UI_console::put(std::string fname, std::string & message)
{
	std::string command = "put|" + fname;
	client->resolve_client();
	client->listen_for_data();
	if (client->sendcommand(command)) 
	{
		client->data_connect();
		long bytes_sent;
		std::string path = client->get_path();
		path += fname;
		FILE *file;
		fopen_s(&file, path.c_str(), "rb");
		if (file != NULL)
		{
			client->sendfile(file, bytes_sent);
			fclose(file);
		}
		else 
		{
			client->sendlong(NULL);
			message = "put failed.";
			client->clean_up_data();
			return false;
		}
		message = "put succeeded.";
		std::cout << "File: " << fname << std::endl;
		std::cout << "Bytes sent: " << bytes_sent << std::endl;
		client->clean_up_data();
		return true;
	}
	else 
	{
		message = "put failed.";
		client->clean_up_data();
		return false;
	}
}

bool UI_Client::UI_console::list(std::string & message)
{
	std::string command = "list";
	client->resolve_client();
	client->listen_for_data();
	if (client->sendcommand(command))
	{
		std::string listing;
		client->data_connect();
		long bytes_read;
		client->readstring(listing, bytes_read);
		message = "list succeeded.";
		std::cout << listing << std::endl;
		std::cout << "Bytes recieved: " << bytes_read << std::endl;
		client->clean_up_data();
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

	
