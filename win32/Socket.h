#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
class Socket
{
	SOCKET u_sock;
	sockaddr_in addr;
	int conn;
public:
	Socket();
	bool Connect(const char * ip, int port);
	bool Send(const char * msg, int length);
	bool Receive(char *& msg);
};

bool checkVersion(void);
#endif