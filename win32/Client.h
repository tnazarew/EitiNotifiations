#ifndef CLIENT_H
#define CLIENT_H
//#include "Socket.h"
#include "Serialization.h"
#include <thread>
class Client : private Serialization
{

public:
    Client();
	bool run(const char * ip_addr, int port);
	
private:
	int MainLoop();
	bool renew();
	//Socket sock;
	std::thread first;
	std::thread second;
	
	int HandlerLoop();
	static BOOL CtrlHandler( DWORD fdwCtrlType );
};

#endif