#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <rc4.h>
class Socket
{
	SOCKET u_sock;
	sockaddr_in addr;
	int conn;
	int devID;
	RC4 sym;

public:
	Socket();
	bool Connect(const char * ip, int port);
	bool Send(const char * msg, int length, int action);
	bool GetSymKey();
	bool Receive(char *& msg);
	bool Reconnect();
	void setDevID(int dev);

	bool SendSymCrypted(char * msg, int length, int action);
	bool ReceiveSymCrypted(char *& msg);
};

bool checkVersion(void);
#endif