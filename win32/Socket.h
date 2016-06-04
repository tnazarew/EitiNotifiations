#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
//#include <rc4.h>
class Socket
{
	SOCKET u_sock;
	sockaddr_in addr;
	int conn;
	int iResult;
	BOOL bResult;
	WSABUF DataBuf;
	const static DWORD EventTotal = 2;
	DWORD RecvBytes;
	DWORD Flags;
	DWORD BytesTransferred;
	WSAEVENT EventArray[EventTotal];
	WSAOVERLAPPED AcceptOverlapped;
	DWORD Index;

public:
	Socket();
	bool Connect(const char * ip, int port);
	int Send(char * msg, int length);
	//bool GetSymKey();
	int Receive(char * msg, int length);
	bool Reconnect();
	//void setDevID(int dev);

	//bool SendSymCrypted(char * msg, int length, int action);
	//bool ReceiveSymCrypted(char *& msg);
};

bool checkVersion(void);
#endif