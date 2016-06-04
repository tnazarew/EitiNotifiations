//#define WIN32_LEAN_AND_MEAN
#include "Client.h"
#include <iostream>
#include <RSA.h>
#include <rc4.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
	int a = -1;
	checkVersion();

	
	//Client client;
	//client.run("192.168.0.102",5444);
	/*
	char * b = "1sdhjfkshfdweurw78";
	char * in;
	char * out;
	RSA::generate();
	RSA::encrypt(b, in);
	RSA::decrypt(in, out);
	std::cout << out;
	*/
	std::string temp("1sdhjfkshfdweurw78");
	char * kk = "ABCDEFGH";
	char * out;
	out = RC4::encrypt(temp, kk);
	std::string newTmp = RC4::decrypt(out, kk);
	std::cout << "msg: " << newTmp << std::endl;
	std::cout << "1: " << temp.size() << ", 2: " << newTmp.size() << std::endl;
	std::cin>>a;
	return 0;
}
