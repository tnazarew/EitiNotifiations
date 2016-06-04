//
// Created by tomasz on 01.05.16.
//

#ifndef EITINOTIFICATIONS_SOCKET_H
#define EITINOTIFICATIONS_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <assert.h>
#include <arpa/inet.h>
#include <netdb.h>
namespace EitiNotifications
{
    typedef struct sockaddr_in sin_addr;
    enum sock_type {SERVER, CLIENT};
    typedef struct sockaddr s_addr;

    class Socket
    {
    private:
        unsigned short port_;
        sock_type socket_type_;
        static const int TYPE = SOCK_STREAM;
        static const int FAMILY = AF_INET;
        int socket_descriptor_;
        int backlog_;
        sin_addr socket_adress_;
        int adress_length_;
    public:
        typedef Socket ClientSocket;
        Socket();
        Socket(int, sock_type, unsigned short);
        Socket(sin_addr, int);
        ~Socket();

        void bindSocket();
        void setPort();
        void connect(int, std::string);
        int readMes(char *&, int&, int&, int&);
        int writeMes(const char *, const int&);
        bool acceptSocket(ClientSocket*&);
        void closeSocket();
        void closeSocket(int);
        int getSocket();
        sin_addr getAdress();
        int setDescriptor();



    };

}

#endif //EITINOTIFICATIONS_SOCKET_H
