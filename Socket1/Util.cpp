#include "Util.h"

void Util::uint32ToHEX(unsigned var, unsigned char hvar[8])
{
	unsigned v = var;
	unsigned b = 0xFU;
	for(int  i = 7; i >= 0; --i)
	{
		b = v&0xF;
		if(b < 10) hvar[i] = '0' + b;
		else hvar[i] = 'A' - 10 + b;
		v >>= 4;
	}
}

void Util::hexToUint32(unsigned char hvar[8], unsigned& var)
{
	var=0;
	for(int  i = 0; i < 7; ++i)
	{
		if(hvar[i] <= '9') var|= (hvar[i] - '0');
		else var|= (hvar[i] - 'A' + 10);
		var <<= 4;
	}
	if(hvar[7] <= '9') var|= (hvar[7] - '0');
		else var|= (hvar[7] - 'A' + 10);
}