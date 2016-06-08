//
// Created by tomasz on 01.05.16.
//
//TODO ADD CLASS FOR CREATING TCP HEADER AND USE HOST TO NET BIT CONVERSION
#ifndef EITINOTIFICATIONS_SERVER_H
#define EITINOTIFICATIONS_SERVER_H

#include <socket.h>
#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <security.h>
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
        Security security_;
        void getConfig(std::vector<std::string>&);
        void closeAll();
        void registerUser(char* enc_mes, int size, Socket* listener);
        void loginUser(char *enc_mes, int size, Socket *listener);
        void clientData();
        void messageRequest();
        void messageRecived();
        void messageRead();
        void sendSuccessLogReg(Socket *listener, const Json::Value &root, const char *session_key, int did) const;
        bool exist();
        void getPublicKey(const Json::Value &root, unsigned int &p_e, unsigned int &p_n) const
        void sendFailLogReg(Socket *listener, const Json::Value &root) const
    public:
        void readAndAnswer(Socket*);
        Server();
        ~Server();
        Server(const Server&);
        void printHex(const char*, int) const;



        int runServer();
        int reconfigure(std::string, int);
        int stopServer();
        int shutdown();

    };
}

#endif //EITINOTIFICATIONS_SERVER_H
