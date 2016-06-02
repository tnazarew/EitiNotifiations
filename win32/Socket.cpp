#include "Socket.h"
#include <iostream>
#include <RSA.h>

Socket::Socket()
{
	iResult = 0;
	bResult = TRUE;
	RecvBytes = Flags = BytesTransferred = 0;
	u_sock = INVALID_SOCKET;
	ZeroMemory(&AcceptOverlapped, sizeof (WSAOVERLAPPED));
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

	EventArray[0] = WSACreateEvent();
    if (EventArray[0] == WSA_INVALID_EVENT) {
		std::cout << "WSACreateEvent failed with error = " << WSAGetLastError() << std::endl;
        closesocket(u_sock);
        WSACleanup();
        return false;
    }

	AcceptOverlapped.hEvent = EventArray[0];

	return true;
}

int Socket::Send(char * data, int len)
{
	DataBuf.buf = data;
	DataBuf.len = RecvBytes = len;
	int smsg = WSASend(u_sock, &DataBuf, 1, &RecvBytes, Flags, &AcceptOverlapped, NULL);
	if(smsg==SOCKET_ERROR){
		std::cout<<"Error while sending: "<<WSAGetLastError()<<std::endl;
		WSACleanup();
	}
	//std::cout << "\nSend: RecvBytes = " << RecvBytes << ", smsg = " << smsg << ", len = " << len+12 << std::endl;
	return RecvBytes;
}

int Socket::Receive(char * msg, int len)
{
	FD_SET readSet;
	FD_ZERO(&readSet);
	FD_SET(u_sock, &readSet);
	
	int count = 0;
	int get = 0;
	while(count<len)
	{
		get=recv(u_sock,msg+count,len,0);
		if(get == SOCKET_ERROR)
		{
			perror("get");
			return false;
		}
		count +=get;
		if(count >= len) break;
		if(select(0,&readSet,NULL,NULL,NULL)==0)
		{
			std::cout<<"select timeout: " << WSAGetLastError() << std::endl;
			break;
		}
		
	}

	return count;
}

/*bool Socket::SendSymCrypted(char * data, int len, int action)
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
}*/

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

/*bool Socket::GetSymKey()
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
}*/

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