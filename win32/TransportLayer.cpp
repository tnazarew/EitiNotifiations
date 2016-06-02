#include "TransportLayer.h"
#include <iostream>

TransportLayer::TransportLayer() : block(NULL), deviceID(0)
{}

int TransportLayer::Send(const char * msg, int length, int action)
{
	block  = new char[length+12];
	*((int*)block) = (length);
	*((int*)(block+4)) = deviceID;
	*((int*)(block+8)) = action;

	if(msg != NULL)
		strcpy(block+12, msg);
	Socket::Send(block, length+12);
	return length+12;
}

int TransportLayer::Receive(char *&msg)
{
	int msize;
	char csize[4];
	Socket::Receive(csize, 4);
	msize = *((int *) csize);
	if(msize > 1500) msize = 1500;
	if(msize < 0) msize = 0;
	if(msize) msg = new char[msize];
	Socket::Receive(msg, msize);
	return msize;
}