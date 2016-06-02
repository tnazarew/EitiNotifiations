#include "Client.h"
#include <iostream>

Client::Client() 
{
	RSA::generate();
}
bool Client::run(const char * ip_addr, int port)
{
	Socket::Connect(ip_addr, port);
	second = std::thread::thread(&Client::HandlerLoop, this);
	first = std::thread::thread(&Client::MainLoop, this);
	first.join();
	second.join();
	
	return true;
}

int Client::MainLoop()
{
	Encrypt::GetSymKey();
	Socket::Reconnect();
	Encrypt::Send(std::string(), 2);
	std::string msg = Encrypt::Receive();
	std::cout << "msg: " << msg << "|size = " << msg.size() << std::endl;
	Socket::Reconnect();
	std::string sstr = Encrypt::checkDigest(msg);
	std::cout << "sstr: " << sstr << std::endl;

	/*char * input;
	// get symmetric key
	sock.GetSymKey();
	// Get messages
	sock.Reconnect();
	sock.Send(NULL, 0, 2);
	sock.ReceiveSymCrypted(input);
	std::cout << "JSON: " << input << std::endl;
	rapidjson::Document doc;
	doc.Parse(input);
	MD5 md5obj;
	if(doc.HasMember("message"))
	{
		std::cout << "Msg: " << doc["message"].GetString() << std::endl;
		std::string str(doc["message"].GetString());
		md5obj.update(str.c_str(), str.size());
		md5obj.finalize();
		std::stringstream dig;
		std::cout << "MD5: " << md5obj << std::endl;
	}
	// send crypted hash
	sock.Reconnect();
	sock.SendSymCrypted(md5obj.hex_digest(), 32, 3);
	sock.ReceiveSymCrypted(input);

	std::cout << "JSON: " << input << std::endl;
	doc.Parse(input);
	if(doc.HasMember("message"))
	{
		std::cout << "Msg: " << doc["message"].GetString() << std::endl;
	}*/

	return 0;
}

int Client::HandlerLoop()
{
	if(!SetConsoleCtrlHandler( (PHANDLER_ROUTINE) Client::CtrlHandler, TRUE ) ) 
	{
		printf( "\nERROR: Could not set control handler"); 
		return 1;
	}
	return 0;
}

BOOL Client::CtrlHandler( DWORD fdwCtrlType )
{
	if(fdwCtrlType == CTRL_C_EVENT)
	{
      printf( "Ctrl-C event\n\n" );
	  
      return( TRUE );
	}
	return FALSE;
}