#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <string>
#include "Client.h"

namespace UI_Client
{
	class UI_console
	{
	public:
		UI_console();
		UI_console(Domain::Client*);
		void mainloop();
		void prompt();
		bool get_command(size_t &end, std::string &command_str);
		bool get_fname(size_t start, std::string input, std::string &fname);
		void remove_space(std::string &text);
		void run_command(std::string command, std::string fname = "");
		bool get(std::string fname, std::string &message);
		bool put(std::string fname, std::string &message);
		bool list(std::string &message);
		bool quit(std::string &message);
		void set_params(std::string h, std::string p);
	private:
		std::string host_name;
		Domain::Client* client;
		std::string port;
		bool quit_loop;

	};
}




#endif // !1
