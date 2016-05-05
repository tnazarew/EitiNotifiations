//
// Created by tomasz on 01.05.16.
//
#include <socket.h>
#include <unistd.h>


namespace EitiNotifications
{
    Socket::Socket()
    {

    }

    Socket::Socket(int backlog, sock_type socket_type, unsigned short port):
    backlog_(backlog), socket_type_(socket_type), port_(port)
    {
        socket_adress_.sin_family = FAMILY;
        socket_adress_.sin_addr.s_addr = htonl(INADDR_ANY);
        socket_adress_.sin_port = htons(port_);
        adress_length_ = sizeof(socket_adress_);
    }

    int Socket::setDescriptor()
    {
        socket_descriptor_ = socket(FAMILY, TYPE, 0);
        assert(socket_descriptor_ >= 0);
        bindSocket();
    }
    Socket::Socket(sin_addr addr, int desc)
    {
        socket_type_ = sock_type::SERVER;
        socket_adress_ = addr;
        socket_descriptor_ = desc;

    }

    Socket::~Socket()
    {

    }

    void Socket::bindSocket()
    {
        int b = bind(socket_descriptor_, (s_addr*)&socket_adress_, adress_length_);
        if(b != 0)
            perror("bind");
        assert(b == 0);

    }


    bool Socket::acceptSocket(ClientSocket& client)
    {
        int b;
        b = listen(socket_descriptor_, backlog_);
        if(b != 0)
            perror("LISTEN");
        assert(b == 0);
        s_addr* s = new s_addr;
        unsigned length = 0;
        int listener_sock = accept(socket_descriptor_, s, &length);
        if (listener_sock == -1)
        {
            perror("ACCEPT");
            return false;
        }
        client = Socket(*((sin_addr*)s), listener_sock);
        delete s;
        return true;
    }

    void Socket::closeSocket()
    {
        close(socket_descriptor_);
    }

    void Socket::closeSocket(int socket)
    {
        close(socket);
    }

    int Socket::getSocket()
    {
        return socket_descriptor_;
    }

    sin_addr Socket::getAdress()
    {
        socket_adress_;
    }



    int Socket::readMes(std::string& mes)
    {
        int long bytes;
        mes.clear();
        const int size = 10;

        do
        {
            char buf[size] = {0};
            bytes = read(socket_descriptor_,buf, size);
            mes.append(buf, 10);
            if(bytes == -1)
            {
                perror("READ");
                mes.clear();
                return -1;
            }
        }while(bytes == size);
        return 0;

    }

    int Socket::writeMes(const std::string& mes)
    {
        write(socket_descriptor_, mes.c_str(), mes.size());
    }


}