#include "Client.h"
#include <iostream>

Client::Client() {}
bool Client::run()
{
	sock.Connect("192.168.0.104",5444);
	char * input;
	std::string msg("Msg From Anton");
	sock.Send(msg.c_str(), msg.size());
	sock.Receive(input);
	std::cout << "Received: " << input << std::endl;
	return true;
}