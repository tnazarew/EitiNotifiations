//
// Created by tomasz on 04.06.16.
//

#include <security.h>
#include <string.h>
#include <netinet/in.h>

namespace EitiNotifications
{

    Security::Security()
    {

    }
    void Security::decryptAsym(const char *input, char *&output, int size)
    {
//        rsa.decrypt(input, output, size);
    }

    bool Security::cypherSym(const int devid, const std::string input, char *&output)
    {
        if(isLogged(devid))
        {
            output = session_keys[devid].encrypt(std::string(input));
            return true;
        }
        return false;
    }

    int Security::encryptAsym(unsigned p_e, unsigned p_n, const std::string &input, char *&output) const
    {
        rsa.encrypt(input, output,p_e, p_n);
    }

    unsigned int Security::encryptAsym(int did, unsigned int p_e, unsigned int p_n) const
    {
        return rsa.encrypt(did, p_e, p_n);
    }

    bool Security::decypherSym(const int devid, const char *input, std::string& output)
    {
        if(isLogged(devid))
        {
            output = session_keys[devid].decrypt(input);
            return true;
        }
        return false;
    }

    bool Security::isLogged(int devid)
    {
//        return session_keys.find(devid) != session_keys.end();
    }

    bool Security::addDevice(int devid, RC4 rc4)
    {
        if(!isLogged(devid))
        {
            session_keys[devid] = rc4;
            return true;
        }
        else
            return false;
    }

    void Security::generateSessionKey(char *&key)
    {
        key = new char[8];
        char *temp = "ABCDEFGH";
        strcpy(key, temp);
    }

    int Security::getDevid()
    {
        return 10;
    }

    char *Security::decryptAsym(const char *enc_mes, int size) const
    {
        char *dec = new char[size];
        for (int i = 0; i < size; i += 4)
        {
            *((unsigned *) (dec + i)) = rsa.decrypt(ntohl(*(unsigned *) (enc_mes + i)));
        }
        return dec;
    }
}