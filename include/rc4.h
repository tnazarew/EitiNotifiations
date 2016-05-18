#ifndef RC4_H
#define RC4_H
struct RC4
{
	// klucz summetryczny
	char * key;
	// zapamietanie klucz symmetrycznego
	bool init(const char * str);
	// syfrowanie deszyfrowanie kluczem symmetrycznym
	void encrypt(const char * ByteInput, char * &ByteOutput);
	static void encrypt(const char * ByteInput, char * pwd, char * &ByteOutput);
	static std::string encrypt(const char * ByteInput, char * pwd);
};
#endif