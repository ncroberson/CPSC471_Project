#include "UI_console.h"
#include <iostream>
#include <algorithm>

UI_Client::UI_console::UI_console(): host_name(""), port(0), quit_loop(false) {}

void UI_Client::UI_console::mainloop()
{
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
	return true;
	message = "get succeeded.";
}

bool UI_Client::UI_console::put(std::string fname, std::string & message)
{
	return true;
	message = "put succeeded.";
}

bool UI_Client::UI_console::list(std::string & message)
{
	return true;
	message = "ls succeeded.";
}

bool UI_Client::UI_console::quit(std::string &message)
{
	quit_loop = true;
	message = "quit succeeded.";
	return true;
}

	
