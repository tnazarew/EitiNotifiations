#include "Client.h"
#include <RSA.h>
#include <iostream>
#include <rc4.h>
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <md5.h>
#include <sstream>

Client::Client() 
{
	RSA::generate();
}
bool Client::run()
{
	int a;
	sock.Connect("192.168.1.29",5444);
	char * input;
	
	sock.GetSymKey();
	sock.Reconnect();
	sock.Send(NULL, 0, 2);
	sock.ReceiveSymCrypted(input);
	std::cout << "JSON: " << input << std::endl;
	rapidjson::Document doc;
	doc.Parse(input);
	MD5 md5obj;
	if(doc.HasMember("message"))
	{
		std::cout << "Msg: " << doc["message"].GetString() << std::endl;
		std::string str(doc["message"].GetString());
		md5obj.update(str.c_str(), str.size());
		md5obj.finalize();
		std::stringstream dig;
		std::cout << "MD5: " << md5obj << std::endl;
	}
	
	sock.Reconnect();
	sock.SendSymCrypted(md5obj.hex_digest(), 32, 3);
	sock.ReceiveSymCrypted(input);

	std::cout << "JSON: " << input << std::endl;
	doc.Parse(input);
	if(doc.HasMember("message"))
	{
		std::cout << "Msg: " << doc["message"].GetString() << std::endl;
	}

	return true;
}