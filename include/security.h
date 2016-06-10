//
// Created by tomasz on 04.06.16.
//

#ifndef EITINOTIFICATIONS_SECURITY_H
#define EITINOTIFICATIONS_SECURITY_H

#include <rc4.h>
#include <map>
#include <RSA.h>
#include <Util.h>
namespace EitiNotifications
{
    static int devid = 0;
    static const int REGISTRATION_REQUEST =0;
    static const int LOGIN_REQUEST =1;
    static const int MESSAGE_REQUEST =2;
    static const int MESSAGE_RECIVED =3;
    static const int MESSAGE_READ =4;
    // API for using cyphers RSA and RC4 and hashing
    class Security
    {
        std::map<int, std::string> session_keys;
        unsigned decryptAsym(unsigned);
        int i;
    public:
        RSA rsa;
        Security();
        bool isLogged(int devid);
        bool cypherSym(const int devid, const std::string input, char*& output);
        char*  encryptAsym(unsigned p_e, unsigned p_n, const std::string& input) const ;
        unsigned int encryptAsym(int did, unsigned int p_e, unsigned int p_n) const;
        bool decypherSym(const int devid, const char* input, int size, std::string& output);
        char* decryptAsym(const char* input, int size) const;
        bool addDevice(int, std::string rc4);
        void generateSessionKey(char *& key);
        int getDevid();
    };


}
#endif //EITINOTIFICATIONS_SECURITY_H
