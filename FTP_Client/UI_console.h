#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <string>

namespace UI_Client
{
	class UI_console
	{
	public:
		UI_console();
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
	private:
		std::string host_name;
		int port;
		bool quit_loop;

	};
}




#endif // !1
