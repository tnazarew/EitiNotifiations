//
// Created by tomasz on 01.05.16.
//

#ifndef EITINOTIFICATIONS_SERVER_H
#define EITINOTIFICATIONS_SERVER_H

#include <socket.h>
#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace EitiNotifications
{
    class Server
    {
    private:
        int port_;
        bool main_thread_;
        bool running;
        s_addr server_addr_;
        Socket* main_socket_;
        std::vector<Socket> sockets_;

        void getConfig(std::vector<std::string>&);
        void closeAll();

    public:
        void readAndAnswer(Socket*);
        Server();
        ~Server();
        Server(const Server&);

        int runServer();
        int reconfigure(std::string, int);
        int stopServer();
        int shutdown();

    };
}

#endif //EITINOTIFICATIONS_SERVER_H
