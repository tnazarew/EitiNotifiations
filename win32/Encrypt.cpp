#include "Encrypt.h"
#include <iostream>

#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
Encrypt::Encrypt()
{
	sym.key = NULL;
	RSA::generate();
}

bool Encrypt::GetSymKey()
{
	char key[8];
	char * input;
	char * key1;
	RSA::pushPublicKey(key);
	if(!TransportLayer::Send(key,8, 1)) return false;
	if(!TransportLayer::Receive(input)) return false;
	RSA::decrypt(input, key1, 8);
	sym.init(key1);
	std::cout << "RC4 key = " << sym.key << std::endl;
	delete input;
	delete key1;
	return true;
}

bool Encrypt::Send(std::string msg, int action)
{
	char * output;
	sym.encrypt(msg.c_str(), output, msg.length());
	if(!TransportLayer::Send(output, msg.length(), action)) return false;
	//delete output;
	return true;
}

std::string Encrypt::Receive()
{
	char * input;
	int size= 0;
	if(!(size = TransportLayer::Receive(input))) return std::string();
	std::cout << "enc input: " << input << "|size = " << size << std::endl;
	sym.encrypt(input, input, size);
	std::cout << "enc input: " << input << "|" << std::endl;
	std::string in(input);
	delete input;
	return in;
}

std::string Encrypt::checkDigest(std::string& msg)
{
	rapidjson::Document doc;
	doc.Parse(msg.c_str());
	std::string str(doc["message"].GetString());
	MD5 md5obj;
	md5obj.update(str.c_str(), str.size());
	md5obj.finalize();
	Send(std::string(md5obj.hex_digest()), 3);
	return Receive();
}