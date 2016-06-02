#ifndef RC4_H
#define RC4_H
#define KEY_SIZE 8 // key size in bytes
#include <string>
struct RC4
{
	// klucz summetryczny
	char * key;
	// zapamietanie klucz symmetrycznego
	bool init(const char * str);
	// syfrowanie deszyfrowanie kluczem symmetrycznym
	void encrypt(const char * ByteInput, char * &ByteOutput, int len = 0);
	static void encrypt(const char * ByteInput, char * pwd, char * &ByteOutput, int len=0);
	static std::string encryptS(const char * ByteInput, char * pwd, int len = 0);
	std::string encryptS(const char * ByteInput, int len = 0);
};
#endif