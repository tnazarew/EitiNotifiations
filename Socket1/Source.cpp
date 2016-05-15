#include <iostream>
#include "Socket.h"
#include "Thread.h"
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	if(!checkVersion())
	{
		std::cout << "Winsock2 not supported.=(" << std::endl;
		Sleep(5000);
		return -1;
	}
	else std::cout << "WSA supported" << std::endl;
	HANDLE socketThread, signalHandlerThread;
	DWORD exitCode;
	DWORD socketThreadID, signalHandlerID;
	socketThread = CreateThread(
		NULL, //default security attributes
		0, // use default stack size
		socketThreadFunction, // thread function name
		NULL, // argument to thread function
		0, // use default creation flags
		&socketThreadID); //returns the thread identifier


	signalHandlerThread = CreateThread(
		NULL, //default security attributes
		0, // use default stack size
		signalHandler, // thread function name
		socketThread, // argument to thread function
		0, // use default creation flags
		&signalHandlerID); //returns the thread identifier

	if(socketThread == NULL)
	{
		std::cout << "Problem with creating socket thread." << std::endl;
		return 3;
	}
	if(signalHandlerThread == NULL)
	{
		std::cout << "Problem with creating signal handler thread." << std::endl;
		return 3;
	}

	WaitForSingleObject(socketThread, INFINITE);
	WaitForSingleObject(signalHandlerThread, INFINITE);
	GetExitCodeThread(socketThread, &exitCode);
	std::cout << "Socket thread exit code: " << exitCode << std::endl;
	GetExitCodeThread(signalHandlerThread, &exitCode);
	std::cout << "Signal handler thread exit code: " << exitCode << std::endl;
	CloseHandle(socketThread);
	CloseHandle(signalHandlerThread);
	std::cin>>exitCode;
    return 0;
}