#include "Thread.h"
#include "Encrypt.h"
#include <string>
HANDLE threadToTerminate;

DWORD WINAPI socketThreadFunction(LPVOID lpParameter)
{
		/*std::cout << "Here1" << std::endl;
		Socket client_sock;
		bool state;

		state = client_sock.Connect("192.168.0.104", 5444);
		if(!state) 
		{
			std::cout << "Connection error" << std::endl;
			return -67;
		}
		std::cout << "Here2" << std::endl;
		std::string message("msg from Anton\n");
		for(int i=0; i<1000; ++i)
		{
			client_sock.Send(std::to_string(i) + std::string(": ") + message);
			client_sock.Receive();
			//client_sock.Reconnect();
			Sleep(5000);
		}
	
		client_sock.Send(std::string("End of transmission\n"));
		client_sock.close();
	std::cout << "Here4" << std::endl;*/
//	ClientEncrypt client;
//	client.Connect("192.168.0.104", 5444);

	return 0;
}

DWORD WINAPI signalHandler(LPVOID lpParameter)
{
	threadToTerminate = (HANDLE) lpParameter;
	if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) ) 
	{ 
		printf( "\nThe Control Handler is installed.\n" ); 
		//printf( "\n -- Now try pressing Ctrl+C or Ctrl+Break, or" ); 
		//printf( "\n    try logging off or closing the console...\n" ); 
		//printf( "\n(...waiting in a loop for events...)\n\n" ); 
	}
	//TerminateThread((HANDLE) lpParameter, 33);
	//std::cout << "Socket must be closed" << std::endl;

	return 0;
}

BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
  switch( fdwCtrlType ) 
  { 
    // Handle the CTRL-C signal. 
    case CTRL_C_EVENT: 
      printf( "Ctrl-C event\n\n" );
      Beep( 750, 300 ); 
	  TerminateThread(threadToTerminate, 33);
      return( TRUE );
 
    // CTRL-CLOSE: confirm that the user wants to exit. 
    case CTRL_CLOSE_EVENT: 
      Beep( 600, 200 ); 
      printf( "Ctrl-Close event\n\n" );
      return( TRUE ); 
 
    // Pass other signals to the next handler. 
    case CTRL_BREAK_EVENT: 
      Beep( 900, 200 ); 
      printf( "Ctrl-Break event\n\n" );
      return FALSE; 
 
    case CTRL_LOGOFF_EVENT: 
      Beep( 1000, 200 ); 
      printf( "Ctrl-Logoff event\n\n" );
      return FALSE; 
 
    case CTRL_SHUTDOWN_EVENT: 
      Beep( 750, 500 ); 
      printf( "Ctrl-Shutdown event\n\n" );
      return FALSE; 
 
    default: 
      return FALSE; 
  } 
}