#include "Serialization.h"
#include <iostream>

Serialization::Serialization() {}

void Serialization::Send(std::string& msg, int action)
{
	std::string newMsg("{\"message\":\"" + msg + "\"}");
	Encrypt::Send(newMsg, action);
}

std::string Serialization::Receive()
{
	std::string msg = Encrypt::Receive();
	rapidjson::Document doc;
	doc.Parse(msg.c_str());
	if(doc.HasMember("message"))
	{
		std::cout << "Msg: " << doc["message"].GetString() << std::endl;
		std::string str(doc["message"].GetString());
		return str;
		//md5obj.update(str.c_str(), str.size());
		//md5obj.finalize();
		//std::stringstream dig;
		//std::cout << "MD5: " << md5obj << std::endl;
	}
	return std::string();
}