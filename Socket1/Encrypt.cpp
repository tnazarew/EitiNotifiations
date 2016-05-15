#include "Encrypt.h"
#include <iostream>
/*
bool ClientEncrypt::Connect(const char * ip_address, int port)
{
	// Client send a public key
	bool state;
	unsigned char e[9] = {0};
	unsigned char n[9] = {0};
	//unsigned nn;
	//unsigned ee;
	Socket client_sock;
	RSA::generate();
	RSA::getPublicKey(e, n);
	state = client_sock.Connect(ip_address, port);
	if(!state) std::cout << "Didn't connect " << std::cout;
	else std::cout << "Connected " << std::cout;
	// send public key to server
	SendPlain(std::string("n")+std::string((char *)e) + std::string((char *)n), client_sock);
	//client_sock.Send(std::string("Msg from Anton.\n"));
	// Server generete aes key
	//new AES keygen();
	// Server send aes key

	// Client gets the aes key
	return true;
}

bool ClientEncrypt::Send(std::string& msg, Socket& sock)
{
	int s = msg.size();
	char s1[2] = {(char)(s&0xFF00), (char)(s&0xFF)};
	unsigned last=0;
	int i;
	unsigned char * str = (unsigned char *) msg.c_str();
	Checksum check;
	sock.Send(s1, 2);
	last = s - msg.size();
	s/=AES::block_size;
	for(i = 0; i < s; ++i)
	{
		unsigned char out[AES::block_size] = {0};
		aesCipher.encrypt(str + i*AES::block_size, out);
		check.crc16((char *)str + i*AES::block_size, AES::block_size);
		sock.Send((char *)out, AES::block_size);
	}
	if(last>0)
	{
		unsigned char out[AES::block_size] = {0};
		aesCipher.encrypt(str + i*AES::block_size, out);
		check.crc16((char *)str + i*AES::block_size, last);
		sock.Send((char *)out, last);
	}
	char a[2];
	check.getChecksum(a);
	sock.Send(a, 2);

	return true;
}

bool ClientEncrypt::SendPlain(std::string& msg, Socket& sock)
{
	Checksum check;
	check.crc16(msg.c_str(), msg.size());
	sock.Send(msg.c_str(), msg.size());
	char a[2];
	check.getChecksum(a);
	sock.Send(a, 2);
	return true;
}

bool ClientEncrypt::ReceiveKey(Socket& sock)
{
	Checksum check;
	unsigned char aes_key[32+2] = {0};
	sock.Receive((char *)aes_key);
	check.crc16((char *)aes_key, 32);
	if (!check.check((char *)(aes_key+32))) return false;
	aesCipher.reconvertKey(aes_key);
	return true;
}

std::string ClientEncrypt::Receive(Socket& sock)
{
	return std::string();
}*/