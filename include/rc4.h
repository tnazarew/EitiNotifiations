#ifndef RC4_H
#define RC4_H
#define KEY_SIZE 8 // key size in bytes
#include <string>
class RC4
{
	// klucz summetryczny
	char * key;
	// syfrowanie deszyfrowanie kluczem symmetrycznym
	static void encrypt(const char * ByteInput, const char * pwd, char * &ByteOutput, size_t length);

public:
	// Client methods:
	RC4();
	bool init(const char * str);
	char * encrypt(std::string msg);
	std::string decrypt(const char * BinaryInput, int length=0);
	// Server methods
	static char * encrypt(std::string msg, const char * givenKey);
	static std::string decrypt(const char * BinaryInput, const char * givenKey,int length=0);
};
#endif