#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include "Encrypt.h"
class Serialization : public Encrypt
{
public:
	Serialization::Serialization();
	void Send(std::string& msg, int action);
	std::string Receive();
};
#endif // SERIALIZATION_H_