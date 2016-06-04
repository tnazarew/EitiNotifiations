#include "Socket.h"
#include <iostream>
#include <RSA.h>

Socket::Socket()
{
}

bool Socket::Connect(const char * ip_address, int port)
{
	int a;

	u_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(u_sock==INVALID_SOCKET)
	{
		std::cout << "Failed on creating socket (socket function)" << WSAGetLastError() << std::endl;
		WSACleanup();
		std::cin>>a;
        return false;
	}

	addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(ip_address);
	addr.sin_port=htons(port);

	conn=connect(u_sock,(SOCKADDR*)&addr,sizeof(addr));
    if(conn==SOCKET_ERROR){
        std::cout<<"Error - when connecting (Connect::connect function) "<<WSAGetLastError()<<std::endl;
        closesocket(u_sock);
        WSACleanup();
		std::cin>> a;
		return false;
	}
	return true;
}

bool Socket::Send(const char * data, int len, int action)
{
	int a;
	char * block  = new char[len+1];
	*((int*)block) = (len);
	*((int*)(block+4)) = devID = 0;
	*((int*)(block+8)) = action;

	if(data != NULL)
		strcpy(block+12, data);
	
	int smsg=send(u_sock,block,len+12,0);
	std::cout<<"Message send was send." << std::endl;
	if(smsg==SOCKET_ERROR){
		std::cout<<"Error while sending: "<<WSAGetLastError()<<std::endl;
		WSACleanup();
	}

	//delete block;
	return true;
}

bool Socket::Receive(char *& msg)
{
	int msize;
	char csize[4];
	FD_SET readSet;
	FD_ZERO(&readSet);
	FD_SET(u_sock, &readSet);
	int get=recv(u_sock,csize,4,0);

	msize = *((int *) csize);;
	if(msize > 1500) msize = 1500;
	if(msize < 0) msize = 0;
	if(get==SOCKET_ERROR){
		std::cout<<"Error in Receiving: "<<WSAGetLastError()<<std::endl;
		std::cin>>msize;
		return false;
	}
	if(msize) msg = new char[msize+1];
	int count = 0;
	while(count<msize)
	{
		get=recv(u_sock,msg+count,msize,0);
		if(get == SOCKET_ERROR)
		{
			perror("get");
			return false;
		}
		count +=get;
		if(count >= msize) break;
		if(select(0,&readSet,NULL,NULL,NULL)==0)
		{
			std::cout<<"select timeout: " << WSAGetLastError() << std::endl;
			break;
		}
		
	}

	msg[msize] = '\0';
	return true;
}

bool Socket::SendSymCrypted(char * data, int len, int action)
{
	char * output;
	sym.encrypt(data, output);
	if(!Send(output, len, action)) return false;
	return true;
}

bool Socket::ReceiveSymCrypted(char *& input)
{
	if(!Receive(input)) return false;
	sym.encrypt(input, input);
	return true;
}

bool Socket::Reconnect()
{
	closesocket(u_sock);
	//return Connect(ip, port);
	u_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(u_sock==INVALID_SOCKET)
	{
		std::cout << "Failed on creating socket (socket function)" << WSAGetLastError() << std::endl;
		WSACleanup();
        return false;
	}
	
	conn=connect(u_sock,(SOCKADDR*)&addr,sizeof(addr));
    if(conn==SOCKET_ERROR){
        std::cout<<"Error - when connecting (Connect::connect function) "<<WSAGetLastError()<<std::endl;
        closesocket(u_sock);
        WSACleanup();
		return false;
	}
	return true;
}

bool Socket::GetSymKey()
{
	char key[8];
	char * input;
	char * key1;
	RSA::pushPublicKey(key);
	if(!Send(key,8, 1)) return false;
	if(!Receive(input)) return false;
	RSA::decrypt(input, key1, 8);
	sym.init(key1);
	std::cout << "RC4 key = " << sym.key << std::endl;
	delete input;
	delete key1;
	return true;
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