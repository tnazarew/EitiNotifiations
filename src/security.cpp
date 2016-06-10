//
// Created by tomasz on 04.06.16.
//

#include <security.h>
#include <string.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>

namespace EitiNotifications
{

    Security::Security()
    {
        i = 10;
    }
//    std::string Security::decryptAsym(const char *input, int size) const
//    {
//        return "";
////        rsa.decrypt(input, output, size);
//    }

    bool Security::cypherSym(const int devid, const std::string input, char *&output)
    {
        if(isLogged(devid))
        {
            output = RC4::encrypt(std::string(input), session_keys[devid].c_str());
            return true;
        }
        return false;
    }

    char* Security::encryptAsym(unsigned p_e, unsigned p_n, const std::string &input) const
    {
        int tempsize = input.size()-1;
        unsigned long mod4 = tempsize % 4;
        unsigned size = (mod4 ? (tempsize ) / 4 + 1 : ((tempsize) / 4));
        char* output = new char[4*size];
        char* in = new char[4*size];
        int temp_mem_size = (mod4 ? 4*size -4 + mod4: 4*size);
        memcpy(in, input.c_str(), temp_mem_size);
        if(mod4!=0)
            for(int i = 0; i < 4-mod4; i++)
                in[4*size - 1 - i] = 0;


        for (int i = 0; i < 4*size; i += 4)
        {
            *((unsigned *) (output + i)) = rsa.encrypt((*(unsigned *) (in + i)), p_e, p_n);
        }
        return output;
    }

    unsigned int Security::encryptAsym(int did, unsigned int p_e, unsigned int p_n) const
    {
        return rsa.encrypt(did, p_e, p_n);
    }

    bool Security::decypherSym(const int devid, const char *input, int size, std::string& output)
    {
        if(isLogged(devid))
        {

            output = RC4::decrypt(input, session_keys[devid].c_str(), size);
            return true;
        }
        return false;
    }

    bool Security::isLogged(int devid)
    {
        return session_keys.find(devid) != session_keys.end();
    }

    bool Security::addDevice(int devid, std::string rc4)
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
        return i++;
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