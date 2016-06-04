//
// Created by tomasz on 01.05.16.
//
#include <server.h>
#include <RSA.h>
#include <rc4.h>
#include <string.h>
#include <unistd.h>
#include <md5.h>

// lepiej przerobic to
enum request {authorization = 1, getMessages};
#define AUTHORIZATION_REQUEST 1
#define GET_MESSAGES 2
#define CHECK_DIGEST 3



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
        //const char* json = "{\"message\":\"Never Gonna Give You Up\"}";
	const char *json = "{\"message\":\"Never Gonna Let You Down!\"}";
        d.Parse(json);
	
        if(listener->readMes(mes, size, devID, action) == 0)
        {
		std::cout << "size = " << size << std::endl;
		std::cout << "devID= " << devID<< std::endl;
		std::cout << "action= " << action << std::endl;
	    if(action == AUTHORIZATION_REQUEST)
	    {
		const char * someKey = "ABCDEFGH";
		char * encryptedSymKey = '\0';
		char * encrypedMessage = '\0';
		int ksize;
	        RSA * asym = new RSA();
		RSA::generate();
	        asym->getPublicKey(mes);
		std::cout<<"partner_e = " << *((unsigned*)mes) << std::endl;
		std::cout<<"partner_n = " << *((unsigned*)(mes+4)) << std::endl;
		asym->encryptWithPPKey(someKey, encryptedSymKey, 8);
		listener->writeMes(encryptedSymKey, 8);
		 
		delete asym;
	    } else if (action == GET_MESSAGES)
	    {
		const char * someKey = "ABCDEFGH";
		char res[] = "{\"message\":\"Never Gonna Let You Down!\"}";
                int res_size = sizeof(res);
		char * encryptedMessage = '\0';
		RC4::encrypt(res, someKey, encryptedMessage);
		std::cout << "res_size = " << res_size << std::endl;
		listener->writeMes(encryptedMessage, res_size);
	    } else if(action == CHECK_DIGEST)
	    {
		const char * someKey = "ABCDEFGH";
		MD5 md5obj;
		char * decryptedMessage = '\0';
		std::string hash(d["message"].GetString());
		md5obj.update(hash.c_str(), hash.size());
		md5obj.finalize();
		std::cout << "MD5 server: " << md5obj << std::endl;
		std::cout << "size = " << hash.size() << std::endl;
		RC4::encrypt(mes, someKey, decryptedMessage);
		std::cout<< decryptedMessage << std::endl;
		char * answer; 
		if(!strncmp(decryptedMessage, md5obj.hex_digest(), 32))
		{ 
		    std::cout << "OK!" << std::endl;
		    answer = "{\"message\":\"OK!\"}";
		}
		else
		{
		    std::cout << "dec: " << decryptedMessage << "|" << std::endl;
		    std::cout << "dig: " << md5obj.hex_digest() << "|" << std::endl;
		    std::cout << "WRONG!" << std::endl;
		    answer = "{\"message\":\"WRONG!\"}";
		}
		RC4::encrypt(answer, someKey, decryptedMessage);
		listener->writeMes(decryptedMessage, strlen(answer));
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
