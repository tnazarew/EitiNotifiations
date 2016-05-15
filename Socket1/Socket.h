#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <string>
/*
 * TCP layer:
 * __________________________________________________________
 * | number of |                              |   checksum: |
 * | bytes     |            D A T A           |    crc-16   |
 * | to accept |                              |             |
 * ----------------------------------------------------------
 * |  2 bytes  |           ? bytes            |    2 bytes  |
 */
// bsd Checksum (16-bit) used for tcp layer
class Checksum
{
	int checksum;
	public:
	Checksum();
	void crc16(const char * str, unsigned length);
	void getChecksum(char a[2]);
	void reset();
	bool check(char a[2]);
};

class Socket
{
public:
	Socket();
	bool Connect(const char * ip_address, int port);
	int Send(const char * data, unsigned len);
	int Receive(char * buffer);
	int Reconnect(void);
	void close();
	~Socket();
private:
	SOCKET u_sock;
	sockaddr_in addr;
	int conn;
};

bool checkVersion(void);

#endif // SOCKET_H
