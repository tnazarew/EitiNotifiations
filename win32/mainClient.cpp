//#define WIN32_LEAN_AND_MEAN
#include "Client.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
	int a = -1;
	checkVersion();

	Client client;
	client.run("192.168.0.102",5444);
   
	std::cin>>a;
	return 0;
}
