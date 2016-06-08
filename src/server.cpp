//
// Created by tomasz on 01.05.16.
//
#include <server.h>


#include <json.h>
//#define 


namespace EitiNotifications
{
    Server::Server()
    {
        main_thread_ = true;
        runServer();
    }

    Server::~Server()
    {
        if (main_thread_)
        {
            perror("Destructor is called");
            delete main_socket_;
        }


    }

    Server::Server(const Server &s)
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
        main_socket_ = new Socket(1, EitiNotifications::sock_type::SERVER, port);
        main_socket_->setDescriptor();
        Socket *new_socket;
        while (running)
        {

            if (main_socket_->acceptSocket(new_socket));
            {
                boost::thread t(boost::bind(&Server::readAndAnswer, *this, new_socket));
            }

        }
    }

    void Server::readAndAnswer(Socket *listener)
    {
        char *mes;
        int size, devID, action;
        if (listener->readMes(mes, size, devID, action) == 0)
        {
            switch (action)
            {
                case REGISTRATION_REQUEST:
                    registerUser(mes, size, listener);
                    break;
                case CLIENT_DATA:
                    clientData();
                    break;
                case LOGIN_REQUEST:
                    loginUser(mes, size, listener);
                    break;
                case MESSAGE_REQUEST:
                    messageRequest();
                    break;
                case MESSAGE_RECIVED:
                    messageRecived();
                    break;
                case MESSAGE_READ:
                    messageRead();
                    break;
            }
        }
        else
            perror("LISTENER");

        close(listener->getSocket());
        delete listener;
    }


    void Server::getConfig(std::vector<std::string> &conf)
    {
        conf.push_back(std::string("127.0.0.1"));
        conf.push_back(std::string("5445"));

    }

    void Server::closeAll()
    {
        for (std::vector<Socket>::iterator i = sockets_.begin(); i != sockets_.end(); i++)
        {
            i->closeSocket();
        }

    }


    void Server::registerUser(char *enc_mes, int size, Socket *listener)
    {
        char *dec = security_.decryptAsym(enc_mes, size);
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(dec, root);     //parse process
        if (!parsingSuccessful)
            std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

        if(!exist())
        {
            char *session_key = new char[8];
            security_.generateSessionKey(session_key);
            int did = security_.getDevid();
            security_.addDevice(did, RC4(session_key));

            sendSuccessLogReg(listener, root, session_key, did);
        }
        else
        {
            sendFailLogReg(listener, root);
        }


    }

    void Server::sendFailLogReg(Socket *listener, const Json::Value &root) const
    {
        unsigned int p_e, p_n;
        char* res;
        getPublicKey(root, p_e, p_n);
        this->security_.rsa.encrypt("Fail", res, p_e, p_n);
        listener->writeMes(res, 4);
    }

    void Server::sendSuccessLogReg(Socket *listener, const Json::Value &root, const char *session_key, int did) const
    {
        unsigned int p_e;
        unsigned int p_n;
        getPublicKey(root, p_e, p_n);
        std::cout << p_e << " " << p_n << std::endl;
        std::vector<char *> res;
        res.resize(3);
        std::string temp(session_key);
        std::string s = temp.substr(0, 4);
        std::vector<int> sizes;
        sizes.push_back(security_.encryptAsym(p_e, p_n, s, res[0]));
        s = temp.substr(4, 4);
        sizes.push_back(security_.encryptAsym(p_e, p_n, s, res[1]));
        unsigned int enc_did = security_.encryptAsym(did, p_e, p_n);

        char *mes = new char[12];
        *(unsigned *) mes = 12;
        *(unsigned *) (mes ) = *(unsigned *) res[0];
        *(unsigned *) (mes + 4) = *(unsigned *) res[1];
        *(unsigned *) (mes + 8) = enc_did;
        printHex(mes, 12);
        listener->writeHeadlessMes(mes, 12);
    }


    void Server::getPublicKey(const Json::Value &root, unsigned int &p_e, unsigned int &p_n) const
    {
        Json::Value c_data = root["cipheredData"];
        std::string s1 = c_data["publicKey"].asString();
        std::string s2 = c_data["modulus"].asString();
        p_e= (unsigned)atoi(s1.c_str());
        p_n= (unsigned)atoi(s2.c_str());
    }


    void Server::printHex(const char *mes, int size) const
    {
        unsigned char *p = (unsigned char *) mes;
        for (int i = 0; i < size; i++)
        {
            printf("0x%02x ", p[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
        printf("\n");
    }


    void Server::loginUser(char *enc_mes, int size, Socket *listener)
    {
        char *dec = security_.decryptAsym(enc_mes, size);
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(dec, root);     //parse process
        if (!parsingSuccessful)
            std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
        if(exist())
        {
            char *session_key = new char[8];
            security_.generateSessionKey(session_key);
            int did = security_.getDevid();
            security_.addDevice(did, RC4(session_key));

            sendSuccessLogReg(listener, root, session_key, did);
        }
        else
        {
            sendFailLogReg(listener, root);
        }

    }

    void Server::clientData()
    {

    }

    void Server::messageRequest()
    {
        //check if deviceid is in the map
        // if is, decrypt and send to database, return messages
    }

    void Server::messageRecived()
    {

    }

    void Server::messageRead()
    {

    }
}