#ifndef UTIL_H
#define UTIL_H
class Util
{
	friend class AES;
	friend class RSA;
	// convert unsigned int to hexademical number in string format
	static void uint32ToHEX(unsigned var, unsigned char hvar[8]);
	// convert to unsigned int from hexademical number in string format
	static void hexToUint32(unsigned char hvar[8], unsigned& var);
};
#endif // UTIL_H