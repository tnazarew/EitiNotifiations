//
// Created by tomasz on 01.05.16.
//
#include <socket.h>
#include <unistd.h>
#include <string.h>




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
        socket_type_ = sock_type::SERVER;
        socket_descriptor_ = socket(FAMILY, TYPE, 0);
        assert(socket_descriptor_ >= 0);
        bindSocket();

    }

    int Socket::setDescriptor()
    {

    }
    Socket::Socket(sin_addr addr, int desc)
    {
        socket_type_ = sock_type::SERVER;
        socket_adress_ = addr;
        socket_descriptor_ = desc;

        socket_type_ = sock_type::CLIENT;
    }

    Socket::~Socket()
    {

        if(socket_type_ == sock_type::SERVER)
            perror("SERVER IS BEING CLOSED");
        closeSocket();

    }

    void Socket::bindSocket()
    {
        int b = bind(socket_descriptor_, (s_addr*)&socket_adress_, adress_length_);
        if(b != 0)
            perror("bind");
        assert(b == 0);

    }


    bool Socket::acceptSocket(ClientSocket* &client)
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
        client = new Socket(*((sin_addr*)s), listener_sock);
        delete s;
        return true;
    }

    void Socket::closeSocket()
    {
        close(socket_descriptor_);

    }


    int Socket::getSocket()
    {
        return socket_descriptor_;
    }

    sin_addr Socket::getAdress()
    {
        socket_adress_;
    }



    int Socket::readMes(char*& mes, int& mes_size, int& devID, int& action)
    {
        const int size = 7;
        int long bytes;
        char * header;
        header = new char[12];
        bytes = read(socket_descriptor_, header, 3*sizeof(int));
        if(bytes == 12)
        {
            mes_size = *((int *) header);
            devID = *((int *) (header + 4));
            action = *((int *) (header + 8));
			if(mes_size<0) mes_size = 1;
			if(mes_size>1500) mes_size = 1500;
            std::cout << "mes_size = " << mes_size << std::endl;
            std::cout << "devID = " << devID << std::endl;
            std::cout << "action = " << action << std::endl;
            mes = new char[mes_size];
            int offset = 0;
            do
            {
                bytes = read(socket_descriptor_, mes + offset, size);
                offset += bytes;
                if (bytes == -1) {
                    perror("READ");
                    return -1;
                }
            } while (offset < mes_size);
            return 0;
        }
        else
        {
            perror("READ HEADER");
                    return -1;
        }
    }

    int Socket::writeMes(const char* mes, const int size)
    {
        char *buf;
        buf = new char[4+size];
        *((int*)buf) = size;
        memcpy(buf+4, mes, (size_t)size);
        send(socket_descriptor_, buf, (size_t)size+4, 0);
    }




}
