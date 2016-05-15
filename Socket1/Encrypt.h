#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "AES.h"
#include "RSA.h"
#include "Socket.h"

class ClientEncrypt
{
	AES aesCipher;
	bool ReceiveKey(Socket& sock);
	bool SendPlain(std::string& msg, Socket& sock);
public:
	bool Connect(const char * ip_address, int port);
	bool Send(std::string& msg, Socket& sock);
	std::string Receive(Socket& sock);
};


#endif // ENCRYPT_H