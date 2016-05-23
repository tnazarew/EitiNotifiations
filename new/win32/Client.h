#ifndef CLIENT_H
#define CLIENT_H
#include "Socket.h"
#include <thread>
class Client
{

public:
    void readAndAnswer();
    Client();
	bool run();

private:
	Socket sock;
	std::thread first;
	std::thread second;

};

#endif