#include "Socket.h"
#include <exception>
#include <sstream>
#include <iostream>

Socket::Socket() {}

bool Socket::Connect(const char * ip_address, int port)
{
	u_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(u_sock==INVALID_SOCKET)
        return false;
    else
        std::cout<<"It was okay to create the socket\n";

	addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(ip_address);
	addr.sin_port=htons(port);

	connect(u_sock,(SOCKADDR*)&addr,sizeof(addr));
    if(conn==SOCKET_ERROR){
        std::cout<<"Error - when connecting "<<WSAGetLastError()<<std::endl;
        closesocket(u_sock);
        WSACleanup();
		return false;
    }
	else std::cout<<"Connected! sock" << std::endl;
	return true;
}

int Socket::Send(const char * data, unsigned len)
{
	int smsg=send(u_sock,data,len,0);
	std::cout<<"Message send: " << data << std::endl;
    if(smsg==SOCKET_ERROR){
        std::cout<<"Error while sending: "<<WSAGetLastError()<<std::endl;
        WSACleanup();
    }
	return smsg;
}

int Socket::Receive(char * buffer)
{
	char vect[512]={0};

		//waitforM
	int get=recv(u_sock,vect,512,0);
    if(get==SOCKET_ERROR){
        std::cout<<"Error in Receiving: "<<WSAGetLastError()<<std::endl;
		return 1;
	}
    std::cout<<"Received: " << vect<<std::endl;
	return 0;
}

int Socket::Reconnect(void)
{
	Socket::close();
	u_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(u_sock==INVALID_SOCKET)
        std::cout<<"Creating socket fail\n";
	connect(u_sock,(SOCKADDR*)&addr,sizeof(addr));
    if(conn==SOCKET_ERROR){
        std::cout<<"Error - when connecting "<<WSAGetLastError()<<std::endl;
        closesocket(u_sock);
        WSACleanup();
    }
	return 0;
}

Socket::~Socket()
{
	closesocket(u_sock);
}

void Socket::close()
{
	closesocket(u_sock);
}

bool checkVersion(void)
{
	WSAData version;
    WORD mkword=MAKEWORD(2,2);
    int what=WSAStartup(mkword,&version);
    if(what!=0){
        std::cout<<"This version is not supported! - \n"<<WSAGetLastError()<<std::endl;
		return false;
    }
    else{
        std::cout<<"Good - Everything fine!\n"<<std::endl;
		return true;
    }
}

Checksum::Checksum() : checksum(0) {};

void Checksum::crc16(const char * str, unsigned length)
{ 
	const char * ptr = str;
	unsigned counter = length;

    while (counter--) {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += *ptr;
		++ptr;
        checksum &= 0xffff;       // Keep it within bounds.
    }
}

void Checksum::reset()
{
	checksum = 0;
}

void Checksum::getChecksum(char a[2])
{
	a[0] = checksum&0xFF00;
	a[1] = checksum&0xFF;
}

bool Checksum::check(char a[2])
{
	int n = 0;
	n|=a[0]; n <<=8; n|= a[1];
	return checksum == n;
}


