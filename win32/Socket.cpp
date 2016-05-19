#include "Socket.h"
#include <iostream>

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
}

bool Socket::Send(const char * data, int len)
{
	char * block  = new char[len+13];
	*((int*)block) = (len);
	*((int*)(block+4)) = 10;
	*((int*)(block+8)) = 12;
	strcpy(block+12, data);
	block[len+12] = '\0';
	std::cout<<"size: " << len << std::endl;
	std::cout<<"Send: "<< block <<std::endl;
	

	int smsg=send(u_sock,block,len+12,0);
	std::cout<<"Message send: " << std::endl;
	if(smsg==SOCKET_ERROR){
		std::cout<<"Error while sending: "<<WSAGetLastError()<<std::endl;
		WSACleanup();
	}

	delete block;
	return true;
}

bool Socket::Receive(char *& msg)
{
	int msize;
	char csize[4];

	std::cout<< "Waiting for receive." << std::endl;
	int get=recv(u_sock,csize,4,0);

	msize = *((int *) csize);
	std::cout<< csize << std::endl;
	std::cout << "size = " << msize << std::endl;
	if(msize > 1000) msize = 1000;
	if(msize < 0) msize = 20;
	if(get==SOCKET_ERROR){
		std::cout<<"Error in Receiving: "<<WSAGetLastError()<<std::endl;
		std::cin>>msize;
		return false;
	}
	msg = new char[msize];
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
	}

	//std::cout<<"Received: " << input.buf<<std::endl;

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