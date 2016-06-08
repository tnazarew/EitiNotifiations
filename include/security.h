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
    static const int CLIENT_DATA =1;
    static const int LOGIN_REQUEST =2;
    static const int MESSAGE_REQUEST =3;
    static const int MESSAGE_RECIVED =4;
    static const int MESSAGE_READ =5;
    // API for using cyphers RSA and RC4 and hashing
    class Security
    {
        std::map<int, RC4> session_keys;
        unsigned decryptAsym(unsigned);
    public:
        RSA rsa;
        Security();
        bool isLogged(int devid);
        bool cypherSym(const int devid, const std::string input, char*& output);
        int  encryptAsym(unsigned p_e, unsigned p_n, const std::string& input, char *&output) const ;
        unsigned int encryptAsym(int did, unsigned int p_e, unsigned int p_n) const;
        bool decypherSym(const int devid, const char* input, std::string& output);
        char* decryptAsym(const char* input, int size) const;
        bool addDevice(int, RC4 rc4);
        void generateSessionKey(char *& key);
        int getDevid();
    };


}
#endif //EITINOTIFICATIONS_SECURITY_H
