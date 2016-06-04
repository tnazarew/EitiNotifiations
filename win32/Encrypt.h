#ifndef ENCRYPT_H_
#define ENCRYPT_H_
#include "TransportLayer.h"
#include <rc4.h>
#include <RSA.h>
#include <md5.h>
class Encrypt : public TransportLayer
{
	RC4 sym;
public:
	Encrypt();
	bool GetSymKey();
	bool Send(std::string msg, int action);
	std::string Receive();
	std::string checkDigest(std::string& str);
};
#endif // ENCRYPT_H_