#ifndef CLIENT_H
#define CLIENT_H

namespace Domain 
{
	class Client 
	{
	public:
		Client();
		init();
		resolve();
		connect();
		send();
		recieve();
		bool senddata(SOCKET sock, void *buf, int buflen);
		bool sendfile(SOCKET sock, FILE *f);
		bool sendlong(SOCKET sock, FILE* f);
	private:

	};


}


#endif
