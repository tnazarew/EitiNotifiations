//
// Created by tomasz on 01.05.16.
//
#include <server.h>



namespace EitiNotifications
{
    Server::Server()
    {
        runServer();
    }
    Server::~Server()
    {
        closeAll();
    }
    int Server::runServer()
    {
        running = true;
        // load configuration
        std::vector<std::string> conf_data;
        getConfig(conf_data);
        unsigned short port = atoi(conf_data[1].c_str());
        // create first socket
        main_socket_= Socket(1, EitiNotifications::sock_type::SERVER, port);
        main_socket_.setDescriptor();
        while(running)
        {
            Socket new_socket;
            if(main_socket_.acceptSocket(new_socket));
//                readAndAnswer(new_socket);
            {
                boost::thread t(boost::bind(&Server::readAndAnswer, *this, new_socket));
//                sockets_.push_back(new_socket);

            }

        }
    }

    void Server::readAndAnswer(Socket& listener)
    {
        std::string mes;
        if(listener.readMes(mes) == 0)
        {
            std::cout << mes << std::endl;
            listener.writeMes(mes);
        }
        else
            perror("LISTENER");

        close(listener.getSocket());
    }



    void Server::getConfig(std::vector<std::string>& conf)
    {
        conf.push_back(std::string("127.0.0.1"));
        conf.push_back(std::string("5444"));

    }
    void Server::closeAll()
    {
        for(std::vector<Socket>::iterator i = sockets_.begin(); i != sockets_.end(); i++)
        {
            i->closeSocket();
        }

    }

}
