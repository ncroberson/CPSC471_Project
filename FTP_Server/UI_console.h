#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include "Server.h"
#include <iostream>

namespace UI_Server
{
	class UI_console
	{
	public:
		UI_console();
		UI_console(Domain::Server*);
		void mainloop();
	private:
		Domain::Server * server;
		bool quit_loop;
	};
}




#endif // !1

