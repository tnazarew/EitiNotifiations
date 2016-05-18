//
// Created by tomasz on 01.05.16.
//
#include <server.h>
#include <RSA.h>
#include <string.h>
#define REGISTER_REQUEST 1



namespace EitiNotifications
{
    Server::Server()
    {
        main_thread_ = true;
        runServer();
    }
    Server::~Server()
    {
        if(main_thread_)
        {
            perror("Destructor is called");
            delete main_socket_;
        }

    }

    Server::Server(const Server& s)
    {
        main_thread_ = false;
    }


    int Server::runServer()
    {
        running = true;
        // load configuration
        std::vector<std::string> conf_data;
        getConfig(conf_data);
        unsigned short port = atoi(conf_data[1].c_str());
        // create first socket
        main_socket_= new Socket(1, EitiNotifications::sock_type::SERVER, port);
        main_socket_->setDescriptor();
        Socket* new_socket;
        while(running)
        {

            if(main_socket_->acceptSocket(new_socket));
            {
                boost::thread t(boost::bind(&Server::readAndAnswer, *this, new_socket));
            }

        }
    }

    void Server::readAndAnswer(Socket* listener)
    {
        char * mes;
        int size, devID, action;
	rapidjson::Document d;
        const char* json = "{\"message\":\"Never Gonna Give You Up\"}";
        d.Parse(json);
        if(listener->readMes(mes, size, devID, action) == 0)
        {
	    if(action == REGISTER_REQUEST)
	    {
		const char * someKey = "ABCDEFGH";
		char * encryptedSymKey = '\0';
		int ksize;
	        RSA * asym = new RSA();
	        asym->getPublicKey(mes);
		asym->encryptWithPPKey(someKey, encryptedSymKey);
		ksize = strlen(encryptedSymKey);
		listener->writeMes(encryptedSymKey, ksize);
		// push back (devID, std::string(someKey) 
		delete asym;
	    }

            else if(d.HasMember("message") && std::string(d["message"].GetString()) == "Never Gonna Give You Up")
            {

                char res[] = "{\"message\":\"Never Gonna Let You Down\"}";
                int res_size = sizeof(res);
                listener->writeMes(res, res_size);
            }
            else
            {
                perror("NO RICKROLL");
            }
        }
        else
            perror("LISTENER");

        close(listener->getSocket());
        delete listener;
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
