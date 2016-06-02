#ifndef TRANSPORTLAYER_H_
#define TRANSPORTLAYER_H_
#include "Socket.h"
class TransportLayer : public Socket
{
	char * block;
	int deviceID;
public:
	TransportLayer();
	//bool Connect(const char * ip, int port);
	int Send(const char * msg, int length, int action);
	int Receive(char *& msg);
	int setDevID(int devID);
};
#endif // TRANSPORTLAYER_H_