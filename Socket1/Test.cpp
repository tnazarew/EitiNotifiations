#include <iostream>
#include "Encrypt.h"
#include <time.h>

int main(void)
{
	int e;
	char ch = -50;
	RSA::generate();
	
	unsigned char decr[256] = {0};
	unsigned char ff[256] = {0};
	unsigned char ini[256] = {0};
	ini[0] = 'S'; ini[1] = 'o'; ini[2] = 'm'; ini[3] = 'e'; ini[4] = ' '; ini[5] = 'm'; ini[6] = 'e'; 
	ini[7] = 's'; ini[8] = 's'; ini[9] = 'a'; ini[10] = 'g'; ini[11] = 'e'; ini[12] = '@'; ini[13] = '!'; 
	//ini[14] = '!'; ini[15] = '!';
	AES aesian;
	clock_t clc = clock();
	aesian.keygen();
	std::cout << "Time = " << clock() - clc << std::endl;
	clc = clock();
	std::cout << "\nMessage: " << ini << std::endl;
	aesian.encrypt(ini, decr);
	std::cout << "\nEncrypted: " << decr << std::endl;
	std::cout << "Int mode: \n";
	for(int i = 0; i < 14; ++i)
		std::cout << (int)(decr[i]) << " ";
	std::cout<< std::endl;
	aesian.decrypt(decr, ff);
	std::cout << "AES decrypted: " << ff << std::endl;
	std::cout << "Time = " << clock() - clc << std::endl;
	aesian.convertKey((unsigned char *)0);

	std::cin>> e;
	return 0;
}
