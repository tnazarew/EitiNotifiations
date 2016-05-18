#include <string.h>
#include <string>
#include "rc4.h"

void RC4::encrypt(const char * ByteInput, char * pwd, char* &ByteOutput)
{
	//unsigned char * temp;
	char * temp;
	int i,j=0,t,tmp,tmp2,s[256], k[256];
	int len = strlen(pwd);
	for (tmp=0;tmp<256;tmp++)
	{
		s[tmp]=tmp;
		k[tmp]=pwd[tmp % len];
	}
	for (i=0;i<256;i++)
	{
		j = ((unsigned char)(j + s[i] + k[i])) % 256;
		tmp=s[i];
		s[i]=s[j];
		s[j]=tmp;
	}
	int len2 = (int)strlen((char *)ByteInput);
	//temp = new unsigned char [ (int)strlen((char *)ByteInput)  + 1 ] ;
	temp = new char[  len2 + 1 ];
	i=j=0;
	for (tmp=0;tmp<len2;tmp++)
	{
	    i = ((unsigned char)(i + 1)) % 256;
        j = ((unsigned char)(j + s[i])) % 256;
		tmp2=s[i];
		s[i]=s[j];
		s[j]=tmp2;
        t = ((unsigned char)(s[i] + s[j])) % 256;
		if (s[t]==ByteInput[tmp])
			temp[tmp]=ByteInput[tmp];
		else
			temp[tmp]=s[t]^ByteInput[tmp];
	}
	temp[tmp]='\0';
	ByteOutput=temp;
}

std::string RC4::encrypt(const char * ByteInput, char * pwd)
{
	char * temp;
	int i,j=0,t,tmp,tmp2,s[256], k[256];
	int len = strlen(pwd);
	for (tmp=0;tmp<256;tmp++)
	{
		s[tmp]=tmp;
		k[tmp]=pwd[tmp % len];
	}
	for (i=0;i<256;i++)
	{
		j = ((unsigned char)(j + s[i] + k[i])) % 256;
		tmp=s[i];
		s[i]=s[j];
		s[j]=tmp;
	}
	int len2 = (int)strlen((char *)ByteInput);
	temp = new char[len2+1];
	i=j=0;
	
	for (tmp=0;tmp<len2;tmp++)
	{
	    i = ((unsigned char)(i + 1)) % 256;
        j = ((unsigned char)(j + s[i])) % 256;
		tmp2=s[i];
		s[i]=s[j];
		s[j]=tmp2;
        t = ((unsigned char)(s[i] + s[j])) % 256;
		if (s[t]==ByteInput[tmp])
			temp[tmp]=ByteInput[tmp];
		else
			temp[tmp]=s[t]^ByteInput[tmp];
	}
	temp[tmp] = '\0';
	return std::string(temp);
}

void RC4::encrypt(const char * ByteInput, char* &ByteOutput)
{
	encrypt(ByteInput, key,ByteOutput);
}

bool RC4::init(const char * str)
{
	key = new char[ (int)strlen((char *)str)];
	strcpy(key, str);
	return true;
}
